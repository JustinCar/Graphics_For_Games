#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	camera = new Camera();
	//quad = Mesh::GenerateQuad();
	terrain = Mesh::GenerateTerrain();
	root = new SceneNode();

	drawCount = 0;

	SkyBox* skyBoxNode = new SkyBox();
	root->AddChild(skyBoxNode);

	Terrain* terrainNode = new Terrain();
	terrainNode->SetMesh(terrain);
	root->AddChild(terrainNode);

	Tree* treeNode = new Tree();
	root->AddChild(treeNode);

	Ocean* oceanNode = new Ocean();
	root->AddChild(oceanNode);

	Rain* rainNode = new Rain();
	root->AddChild(rainNode);

	started = false;
	timeAtStart = 0.0f;

	camera->SetPosition(Vector3(100 * 10,
		100.0f, 100 * 10));

	/*light = new Light(Vector3((1190),
		500.0f, 1250),
		Vector4(1.0f, 1.0f, 1.0f, 1),
		(1000 * 500));*/

	light = new Light(Vector3(550,
		500.0f, 2670),
		Vector4(1.0f, 1.0f, 1.0f, 1),
		(1000 * 500));


	reflectShader = new Shader(SHADERDIR "PerPixelVertex.glsl",
		SHADERDIR "reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl",
		SHADERDIR "skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"CourseWork/GrowTerrainVertexShadow.glsl",
		SHADERDIR "CourseWork/TerrainFragmentShadow.glsl");
	treeShader = new Shader(SHADERDIR "CourseWork/TreeGrowVertex.glsl",
		SHADERDIR "CourseWork/TreeGrowFragment.glsl");
	shadowShader = new Shader(SHADERDIR "shadowVert.glsl",
		SHADERDIR "shadowFrag.glsl");
	rainShader = new Shader(SHADERDIR "CourseWork/RainVertex.glsl",
		SHADERDIR "CourseWork/RainFragment.glsl",
		SHADERDIR "CourseWork/RainGeom.glsl");

	terrainHeightMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/heightMapVSmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);


	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR "Coursework/Skybox/Left.png", TEXTUREDIR "Coursework/Skybox/Right.png",
		TEXTUREDIR "Coursework/Skybox/Up.png", TEXTUREDIR "Coursework/Skybox/Down.png",
		TEXTUREDIR "Coursework/Skybox/Front.png", TEXTUREDIR "Coursework/Skybox/Back.png",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);
	

	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() ||
		!skyboxShader->LinkProgram() || !treeShader->LinkProgram() || !shadowShader->LinkProgram()
		|| !rainShader->LinkProgram()) {
		return;
	}

	terrainNode->SetLight(light);
	terrainNode->SetCamera(camera);
	terrainNode->SetShader(lightShader);
	terrainNode->SetHeightMap(&terrainHeightMap);
	terrainNode->setCastShadows();
	terrainNode->setRecieveShadows();

	treeNode->SetLight(light);
	treeNode->SetCamera(camera);
	treeNode->SetShader(treeShader);
	treeNode->SetHeightMap(&terrainHeightMap);
	treeNode->setCastShadows();
	treeNode->setRecieveShadows();

	oceanNode->SetLight(light);
	oceanNode->SetCamera(camera);
	oceanNode->SetShader(reflectShader);
	oceanNode->setRecieveShadows();
	oceanNode->SetCubeMap(&cubeMap);

	skyBoxNode->SetShader(skyboxShader);
	skyBoxNode->SetCubeMap(&cubeMap);

	rainNode->SetShader(rainShader);
	rainNode->SetLight(light);

	if (!cubeMap) {
		return;
	}

	std::vector<Mesh*> treeChildren = treeNode->GetOBJ()->getChildren();


	SetTextureRepeating(treeNode->GetMesh()->GetTexture(), true);
	SetTextureRepeating(treeNode->GetMesh()->GetBumpMap(), true);

	SetTextureRepeating(treeChildren.at(0)->GetTexture(), true);
	SetTextureRepeating(treeChildren.at(0)->GetBumpMap(), true);

	SetTextureRepeating(oceanNode->GetMesh()->GetTexture(), true);
	SetTextureRepeating(terrainNode->GetStone(), true);
	SetTextureRepeating(terrainNode->GetGrass(), true);
	SetTextureRepeating(terrainNode->GetSnow(), true);
	SetTextureRepeating(terrainNode->GetWater(), true);
	SetTextureRepeating(terrainNode->GetLava(), true);
	SetTextureRepeating(terrainNode->GetGrassBump(), true);
	SetTextureRepeating(terrainNode->GetStoneBump(), true);
	SetTextureRepeating(terrainNode->GetSnowBump(), true);

	// Shadow mapping ---------------------------
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	init = true;
	waterRotate = 0.0f;

	projMatrix = Matrix4::Perspective(1.0f, 4000.0f,
		(float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

}

Renderer ::~Renderer(void) {
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);

	delete camera;
	delete terrain;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	delete shadowShader;

	currentShader = 0;
}

void Renderer::reloadShaders() 
{

	reflectShader = new Shader(SHADERDIR "PerPixelVertex.glsl",
		SHADERDIR "reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl",
		SHADERDIR "skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"CourseWork/GrowTerrainVertexShadow.glsl",
		SHADERDIR "CourseWork/TerrainFragmentShadow.glsl");
	treeShader = new Shader(SHADERDIR "CourseWork/TreeGrowVertex.glsl",
		SHADERDIR "CourseWork/TreeGrowFragment.glsl");
	rainShader = new Shader(SHADERDIR "CourseWork/RainVertex.glsl",
		SHADERDIR "CourseWork/RainFragment.glsl",
		SHADERDIR "CourseWork/RainGeom.glsl");


	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() ||
		!skyboxShader->LinkProgram() || !treeShader->LinkProgram() || !shadowShader->LinkProgram()
		|| !rainShader->LinkProgram()) {
		return;
	}
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {

		light->SetPosition(light->GetPosition() + Vector3(10, 0, 0));
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN)) {
		light->SetPosition(light->GetPosition() + Vector3(-10, 0, 0));
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT)) {
		light->SetPosition(light->GetPosition() + Vector3(0, 0, 10));
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT)) {
		light->SetPosition(light->GetPosition() + Vector3(0, 0, -10));
	}


	viewMatrix = camera->BuildViewMatrix();
	waterRotate += msec / 3000.0f;
}

void Renderer::RenderScene(float msec) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_P)) {
		cout << camera->GetPosition() << std::endl;
	}


	if (Window::GetKeyboard()->KeyDown(KEYBOARD_RETURN) && !started) {
		timeAtStart = msec;
		drawCount++;
	}

	float time = msec - timeAtStart;


	DrawShadowScene(time); // First render pass ...
	DrawCombinedScene(time); // Second render pass ...


	SwapBuffers();
}

void Renderer::DrawShadowScene(float msec) {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	Matrix4 temp = projMatrix;

	projMatrix = Matrix4::Perspective(10.0f, 5000.0f,
		1, 45.0f);

	SetCurrentShader(shadowShader);
	viewMatrix = Matrix4::BuildViewMatrix(
		light->GetPosition(), Vector3(0, 0, 0));

	shadowMatrix = biasMatrix * (projMatrix * viewMatrix);

	UpdateShaderMatrices();

	for (vector < SceneNode* >::const_iterator i =
		root -> GetChildIteratorStart();
		i != root -> GetChildIteratorEnd(); ++i) {
		if (!(*i)->getCastShadows())
		{
			continue;
		}
		(*i)->Draw(*this, msec, shadowTex, drawCount);
	}

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	projMatrix = temp;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawCombinedScene(float msec) {

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_L)) {
		projMatrix = Matrix4::Perspective(10.0f, 5000.0f,
			1, 45.0f);
		viewMatrix = Matrix4::BuildViewMatrix(
			light->GetPosition(), Vector3(0, 0, 0));

		cout << light->GetPosition() << std::endl;
	}
	else {
		projMatrix = Matrix4::Perspective(10.0f, 5000.0f,
			(float)width / (float)height, 45.0f);

		viewMatrix = camera->BuildViewMatrix();
	}

	for (vector < SceneNode* >::const_iterator i =
		root->GetChildIteratorStart();
		i != root->GetChildIteratorEnd(); ++i) {
		(*i)->Draw(*this, msec, shadowTex, drawCount);
	}

	glUseProgram(0);
}