#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	camera = new Camera();
	quad = Mesh::GenerateQuad();
	terrain = new Terrain();

	tree = new OBJMesh(MESHDIR "CourseWork/TreeHD.obj");

	for (int i = 0; i < 100; i++) 
	{
		treePositions[i][0] = rand() % 1000 + 1;
		treePositions[i][1] = rand() % 1000 + 1;
		treeSizes[i][0] = rand() % 3;
	}
	
	started = false;
	timeAtStart = 0.0f;

	camera->SetPosition(Vector3(100 * 10,
		100.0f, 100 * 10));

	light = new Light(Vector3((1190),
		500.0f, 1250),
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

	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() ||
		!skyboxShader->LinkProgram() || !treeShader->LinkProgram() || !shadowShader->LinkProgram()) {
		return;
	}

	tree->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/Forest/bark01_bottom.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	tree->GenerateNormals();

	std::vector<Mesh*> treeChildren = tree->getChildren();
	treeChildren[0]->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/Forest/tree_branches.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	treeChildren[0]->GenerateNormals();

	tree->SetBumpMap(SOIL_load_OGL_texture(
		TEXTUREDIR "Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	treeChildren[0]->SetBumpMap(SOIL_load_OGL_texture(
		TEXTUREDIR "Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/lava.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR "Coursework/Left_Space.png", TEXTUREDIR "Coursework/Right_Space.png",
		TEXTUREDIR "Coursework/Up_Space.png", TEXTUREDIR "Coursework/Down_Space.png",
		TEXTUREDIR "Coursework/Front_Space.png", TEXTUREDIR "Coursework/Back_Space.png",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);

	if (!cubeMap || !quad->GetTexture() || !terrain->GetStone()) {
		return;
	}

	SetTextureRepeating(tree->GetTexture(), true);
	SetTextureRepeating(tree->GetBumpMap(), true);

	SetTextureRepeating(treeChildren.at(0)->GetTexture(), true);
	SetTextureRepeating(treeChildren.at(0)->GetBumpMap(), true);

	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(terrain->GetStone(), true);
	SetTextureRepeating(terrain->GetGrass(), true);
	SetTextureRepeating(terrain->GetSnow(), true);
	SetTextureRepeating(terrain->GetWater(), true);
	SetTextureRepeating(terrain->GetLava(), true);
	SetTextureRepeating(terrain->GetGrassBump(), true);
	SetTextureRepeating(terrain->GetStoneBump(), true);
	SetTextureRepeating(terrain->GetSnowBump(), true);

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

	projMatrix = Matrix4::Perspective(100.0f, 5000.0f,
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
	delete tree;
	delete quad;
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


	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() ||
		!skyboxShader->LinkProgram() || !treeShader->LinkProgram()) {
		return;
	}
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_RETURN)) {
		if (!started)
		{
			timeAtStart = msec;
			started = true;
		}
			
		
	}

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

	DrawSkybox();

	float time = msec - timeAtStart;

	if (started) 
	{
		DrawShadowScene(time); // First render pass ...
		DrawCombinedScene(time); // Second render pass ...
	}
	
	DrawLava();

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

	for (int i = 0; i < 100; i++)
	{
		DrawTree(msec, i);
	}
	//DrawTree(msec, 4);
	DrawTerrain(msec);

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
		projMatrix = Matrix4::Perspective(100.0f, 5000.0f,
			(float)width / (float)height, 45.0f);

		viewMatrix = camera->BuildViewMatrix();
	}

	for (int i = 0; i < 100; i++)
	{
		DrawTree(msec, i);
	}
	//DrawTree(msec, 4);
	DrawTerrain(msec);

	glUseProgram(0);
}

void Renderer::DrawTree(float msec, int index)
{
	SetCurrentShader(treeShader);
	SetShaderLight(*light);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader -> GetProgram(),
		"diffuseTex"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tree->GetBumpMap());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
			"bumpTex"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, terrain->GetHeightMap());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "heightMap"), 2);

	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"shadowTex"), 11);
	

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"time"), msec);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"xPos"), treePositions[index][0]);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"zPos"), treePositions[index][1]);

	modelMatrix = Matrix4::Translation(Vector3(treePositions[index][0], 0, treePositions[index][1])) * Matrix4::Scale(Vector3(treeSizes[index][0], treeSizes[index][0], treeSizes[index][0]));

	Matrix4 tempMatrix = shadowMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "shadowMatrix"), 1, false, *&tempMatrix.values);


	UpdateShaderMatrices();
	
	tree->Draw();

	glUseProgram(0);
}

void Renderer::DrawTerrain(float msec) {
	SetCurrentShader(lightShader);
	SetShaderLight(*light);

	modelMatrix.ToIdentity();

	//textureMatrix.ToIdentity();

	UpdateShaderMatrices();


	Matrix4 tempMatrix = shadowMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader -> GetProgram()
		, "shadowMatrix"), 1, false, *&tempMatrix.values);


	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)& camera->GetPosition());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrain->GetStone());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"stoneTex"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrain->GetGrass());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"grassTex"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, terrain->GetSnow());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"snowTex"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, terrain->GetWater());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"waterTex"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrain->GetLava());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"lavaTex"), 4);


	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"time"), msec);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, terrain->GetHeightMap());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "heightMap"), 6);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, terrain->GetGrassMap());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "grassMap"), 7);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, terrain->GetStoneMap());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "stoneMap"), 8);

	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, terrain->GetSnowMap());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "snowMap"), 9);


	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, terrain->GetGrassBump());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"grassBump"), 10);

	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"shadowTex"), 11);

	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, terrain->GetStoneBump());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"stoneBump"), 12);

	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, terrain->GetSnowBump());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"snowBump"), 13);

	terrain->Draw();

	glUseProgram(0);
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

void Renderer::DrawLava() {
	SetCurrentShader(reflectShader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (1000 / 2.0f);

	float heightY = 50;

	float heightZ = (1000 / 2.0f);

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX * 2, 1, heightZ * 2)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(50.0f, 50.0f, 50.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
}
