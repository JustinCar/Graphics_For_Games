#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	camera = new Camera();
	terrain = Mesh::GenerateTerrain();
	root = new SceneNode();

	drawCount = -1;
	startTime = 30;

	skyBoxNode = new SkyBox();
	root->AddChild(skyBoxNode);

	terrainNode = new Terrain();
	terrainNode->SetMesh(terrain);
	root->AddChild(terrainNode);

	treeNode = new Tree();
	root->AddChild(treeNode);

	oceanNode = new Ocean();
	root->AddChild(oceanNode);

	rainNode = new Rain();
	root->AddChild(rainNode);

	lightningNode = new Lightning();
	lightningNode->SetTerrain(terrainNode);
	lightningNode->SetOcean(oceanNode);
	lightningNode->SetTree(treeNode);
	root->AddChild(lightningNode);

	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);
	hellData->AddAnim(MESHDIR"idle2.md5anim");
	hellNode->PlayAnim(MESHDIR"idle2.md5anim");

	hellKnightNode = new HellKnight();

	hellKnightNode->SetHellNode(hellNode);
	root->AddChild(hellKnightNode);

	started = false;
	elapsedTime = 0.0f;

	camera->SetPosition(Vector3(100 * 10,
		100.0f, 100 * 10));

	light = new Light(Vector3(550,
		500.0f, 2670),
		Vector4(1.0f, 1.0f, 1.0f, 1),
		(1000 * 500));

	reflectShader = new Shader(SHADERDIR "CourseWork/WaterVertex.glsl",
		SHADERDIR "CourseWork/WaterFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"CourseWork/skyboxVertex.glsl",
		SHADERDIR "CourseWork/skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"CourseWork/GrowTerrainVertexShadow.glsl",
		SHADERDIR "CourseWork/TerrainFragmentShadow.glsl");
	treeShader = new Shader(SHADERDIR "CourseWork/TreeGrowVertex.glsl",
		SHADERDIR "CourseWork/TreeGrowFragment.glsl");
	shadowShader = new Shader(SHADERDIR "shadowVert.glsl",
		SHADERDIR "shadowFrag.glsl");
	rainShader = new Shader(SHADERDIR "CourseWork/RainVertex.glsl",
		SHADERDIR "CourseWork/RainFragment.glsl",
		SHADERDIR "CourseWork/RainGeom.glsl");
	lightningShader = new Shader(SHADERDIR "CourseWork/lightningVertex.glsl",
		SHADERDIR "CourseWork/lightningFragment.glsl",
		SHADERDIR "CourseWork/lightningGeom.glsl");
	hellShader = new Shader(SHADERDIR"skeletonVertexSimple.glsl", 
		SHADERDIR"skeletonFragment.glsl");

	terrainHeightMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/heightMapVSmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);


	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR "Coursework/Skybox/Left.png", TEXTUREDIR "Coursework/Skybox/Right.png",
		TEXTUREDIR "Coursework/Skybox/Up.png", TEXTUREDIR "Coursework/Skybox/Down.png",
		TEXTUREDIR "Coursework/Skybox/Front.png", TEXTUREDIR "Coursework/Skybox/Back.png",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);

	cubeMapFog = SOIL_load_OGL_cubemap(
		TEXTUREDIR "Coursework/Skybox/FoggySky/Left.png", TEXTUREDIR "Coursework/Skybox/FoggySky/Right.png",
		TEXTUREDIR "Coursework/Skybox/FoggySky/Up.png", TEXTUREDIR "Coursework/Skybox/FoggySky/Down.png",
		TEXTUREDIR "Coursework/Skybox/FoggySky/Front.png", TEXTUREDIR "Coursework/Skybox/FoggySky/Back.png",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);
	

	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() ||
		!skyboxShader->LinkProgram() || !treeShader->LinkProgram() || !shadowShader->LinkProgram()
		|| !rainShader->LinkProgram() || !lightningShader->LinkProgram() || !hellShader->LinkProgram()) {
		return;
	}

	terrainNode->SetLight(light);
	terrainNode->SetCamera(camera);
	terrainNode->SetShader(lightShader);
	terrainNode->SetHeightMap(&terrainHeightMap);
	terrainNode->setCastShadows();

	treeNode->SetLight(light);
	treeNode->SetCamera(camera);
	treeNode->SetShader(treeShader);
	treeNode->SetHeightMap(&terrainHeightMap);
	treeNode->setCastShadows();

	oceanNode->SetLight(light);
	oceanNode->SetCamera(camera);
	oceanNode->SetShader(reflectShader);
	oceanNode->SetCubeMap(&cubeMap);
	oceanNode->SetCubeMapFog(&cubeMapFog);

	skyBoxNode->SetShader(skyboxShader);
	skyBoxNode->SetCubeMap(&cubeMap);
	skyBoxNode->SetCubeMapFog(&cubeMapFog);

	rainNode->SetShader(rainShader);
	rainNode->SetLight(light);

	lightningNode->SetShader(lightningShader);
	lightningNode->SetCamera(camera);

	hellKnightNode->SetShader(hellShader);

	if (!cubeMap) {
		return;
	}

	std::vector<Mesh*> treeChildren = treeNode->GetOBJ()->getChildren();

	SetTextureRepeating(treeNode->GetMesh()->GetTexture(), true);
	SetTextureRepeating(treeNode->GetMesh()->GetBumpMap(), true);

	SetTextureRepeating(treeChildren.at(0)->GetTexture(), true);
	SetTextureRepeating(treeChildren.at(0)->GetBumpMap(), true);

	SetTextureRepeating(oceanNode->GetMesh()->GetTexture(), true);
	SetTextureRepeating(oceanNode->GetMesh()->GetBumpMap(), true);
	SetTextureRepeating(terrainNode->GetStone(), true);
	SetTextureRepeating(terrainNode->GetGrass(), true);
	SetTextureRepeating(terrainNode->GetSnow(), true);
	SetTextureRepeating(terrainNode->GetWater(), true);
	SetTextureRepeating(terrainNode->GetLava(), true);
	SetTextureRepeating(terrainNode->GetGrassBump(), true);
	SetTextureRepeating(terrainNode->GetStoneBump(), true);
	SetTextureRepeating(terrainNode->GetSnowBump(), true);

	SetTextureRepeating(lightningNode->GetMesh()->GetTexture(), true);

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

	projMatrix = Matrix4::Perspective(10.0f, 4000.0f,
		(float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//glEnable(GL_CULL_FACE);

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
	delete hellData;
	delete hellNode;
	delete rainShader;
	delete lightningShader;
	delete hellShader;

	currentShader = 0;
}

void Renderer::reloadShaders() 
{

	reflectShader = new Shader(SHADERDIR "CourseWork/WaterVertex.glsl",
		SHADERDIR "CourseWork/WaterFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"CourseWork/skyboxVertex.glsl",
		SHADERDIR "CourseWork/skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"CourseWork/GrowTerrainVertexShadow.glsl",
		SHADERDIR "CourseWork/TerrainFragmentShadow.glsl");
	treeShader = new Shader(SHADERDIR "CourseWork/TreeGrowVertex.glsl",
		SHADERDIR "CourseWork/TreeGrowFragment.glsl");
	shadowShader = new Shader(SHADERDIR "shadowVert.glsl",
		SHADERDIR "shadowFrag.glsl");
	rainShader = new Shader(SHADERDIR "CourseWork/RainVertex.glsl",
		SHADERDIR "CourseWork/RainFragment.glsl",
		SHADERDIR "CourseWork/RainGeom.glsl");
	lightningShader = new Shader(SHADERDIR "CourseWork/lightningVertex.glsl",
		SHADERDIR "CourseWork/lightningFragment.glsl",
		SHADERDIR "CourseWork/lightningGeom.glsl");
	hellShader = new Shader(SHADERDIR"skeletonVertexSimple.glsl",
		SHADERDIR"skeletonFragment.glsl");


	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() ||
		!skyboxShader->LinkProgram() || !treeShader->LinkProgram() || !shadowShader->LinkProgram()
		|| !rainShader->LinkProgram() || !lightningShader->LinkProgram() || !hellShader->LinkProgram()) {
		return;
	}

	terrainNode->SetShader(lightShader);
	treeNode->SetShader(treeShader);
	oceanNode->SetShader(reflectShader);
	skyBoxNode->SetShader(skyboxShader);
	rainNode->SetShader(rainShader);
	lightningNode->SetShader(lightningShader);
	hellKnightNode->SetShader(hellShader);
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
	hellNode->Update(msec);

	viewMatrix = camera->BuildViewMatrix();
	waterRotate += msec / 3000.0f;
}

void Renderer::RenderScene(float msec) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	msec -= elapsedTime;

	float seconds = msec / 1000;

	if (seconds > startTime)
		drawCount = 1;

	if (seconds > startTime + 20)
		drawCount = 2;

	if (seconds > startTime + 25)
		drawCount = 3;

	if (seconds > startTime + 30)
		drawCount = 4;

	// reset
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_RETURN)) {
		elapsedTime = msec - startTime * 1000;
		drawCount = -1;
	}

	DrawShadowScene(msec); // First render pass ...
	DrawCombinedScene(msec, 0); // Second render pass ...

	if (drawCount < 3) 
	{
		glViewport(width - width / 4, height - height / 4, width / 4, height / 4);
		DrawCombinedScene(msec, 1); // Second render pass ...
	}

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

void Renderer::DrawCombinedScene(float msec, int num) {

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_L)) {
		projMatrix = Matrix4::Perspective(10.0f, 5000.0f,
			1, 45.0f);
		viewMatrix = Matrix4::BuildViewMatrix(
			light->GetPosition(), Vector3(0, 0, 0));

		cout << camera->GetPosition() << std::endl;
	}
	else {
		projMatrix = Matrix4::Perspective(10.0f, 5000.0f,
			(float)width / (float)height, 45.0f);

		if (num == 0) 
			viewMatrix = camera->BuildViewMatrix();
		else 
			viewMatrix = Matrix4::BuildViewMatrix(Vector3(500.0f, 1000.0f, 500.0f), Vector3(500.0f, 0.0f, 500.0f), Vector3(1.0f, 0.0f, 0.0f));
		
	}

	for (vector < SceneNode* >::const_iterator i =
		root->GetChildIteratorStart();
		i != root->GetChildIteratorEnd(); ++i) {
		(*i)->Draw(*this, msec, shadowTex, drawCount);
	}

	glUseProgram(0);
}