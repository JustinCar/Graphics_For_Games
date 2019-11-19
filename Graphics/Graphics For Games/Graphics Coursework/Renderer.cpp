#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	camera = new Camera();
	quad = Mesh::GenerateQuad();
	terrain = Mesh::GenerateTerrain();
	root = new SceneNode();

	Terrain* terrainNode = new Terrain();	terrainNode->SetMesh(terrain);	root->AddChild(terrainNode);	Tree* treeNode = new Tree();	root->AddChild(treeNode);
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

	terrainHeightMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/heightMapVSmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	

	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() ||
		!skyboxShader->LinkProgram() || !treeShader->LinkProgram() || !shadowShader->LinkProgram()) {
		return;
	}

	terrainNode->SetLight(light);
	terrainNode->SetCamera(camera);
	terrainNode->SetShader(lightShader);
	terrainNode->SetHeightMap(&terrainHeightMap);

	treeNode->SetLight(light);
	treeNode->SetCamera(camera);
	treeNode->SetShader(treeShader);
	treeNode->SetHeightMap(&terrainHeightMap);

	/*tree->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/Forest/bark01_bottom.tga",
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
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));*/

	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/lava.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR "Coursework/Left_Space.png", TEXTUREDIR "Coursework/Right_Space.png",
		TEXTUREDIR "Coursework/Up_Space.png", TEXTUREDIR "Coursework/Down_Space.png",
		TEXTUREDIR "Coursework/Front_Space.png", TEXTUREDIR "Coursework/Back_Space.png",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);

	if (!cubeMap || !quad->GetTexture()) {
		return;
	}

	std::vector<Mesh*> treeChildren = treeNode->GetOBJ()->getChildren();


	SetTextureRepeating(treeNode->GetMesh()->GetTexture(), true);
	SetTextureRepeating(treeNode->GetMesh()->GetBumpMap(), true);

	SetTextureRepeating(treeChildren.at(0)->GetTexture(), true);
	SetTextureRepeating(treeChildren.at(0)->GetBumpMap(), true);

	SetTextureRepeating(quad->GetTexture(), true);
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
	//delete tree;
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

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_RETURN) && !started) {
		timeAtStart = msec;
		started = true;
	}

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

	/*for (int i = 0; i < numTrees; i++)
	{
		DrawTree(msec, i);
	}*/

	for (vector < SceneNode* >::const_iterator i =
		root -> GetChildIteratorStart();
		i != root -> GetChildIteratorEnd(); ++i) {
		(*i)->Draw(*this, msec, shadowTex);	}
	//DrawTerrain(msec);

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

	/*for (int i = 0; i < numTrees; i++)
	{
		DrawTree(msec, i);
	}*/
	for (vector < SceneNode* >::const_iterator i =
		root->GetChildIteratorStart();
		i != root->GetChildIteratorEnd(); ++i) {
		(*i)->Draw(*this, msec, shadowTex);	}
	//DrawTerrain(msec);

	glUseProgram(0);
}

//void Renderer::DrawTree(float msec, int index)
//{
//	SetCurrentShader(treeShader);
//	SetShaderLight(*light);
//
//	modelMatrix.ToIdentity();
//	textureMatrix.ToIdentity();
//
//	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
//		"cameraPos"), 1, (float*)& camera->GetPosition());
//
//	glUniform1i(glGetUniformLocation(currentShader -> GetProgram(),
//		"diffuseTex"), 0);
//
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, tree->GetBumpMap());
//	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
//			"bumpTex"), 1);
//
//	glActiveTexture(GL_TEXTURE2);
//	glBindTexture(GL_TEXTURE_2D, terrainHeightMap);
//	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "heightMap"), 2);
//
//	glActiveTexture(GL_TEXTURE11);
//	glBindTexture(GL_TEXTURE_2D, shadowTex);
//	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
//		"shadowTex"), 11);
//	
//
//	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
//		"time"), msec);
//
//	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
//		"xPos"), treePositions[index][0] * TEXTURE_SEPARATION);
//
//	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
//		"zPos"), treePositions[index][1] * TEXTURE_SEPARATION);
//
//
//	modelMatrix = Matrix4::Translation(Vector3(treePositions[index][0], 0, treePositions[index][1])) * Matrix4::Scale(Vector3(1, 1, 1));
//
//	Matrix4 tempMatrix = shadowMatrix * modelMatrix;
//
//	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
//		,"shadowMatrix"), 1, false, *&tempMatrix.values);
//
//
//	UpdateShaderMatrices();
//	
//	tree->Draw();
//
//	glUseProgram(0);
//}

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
