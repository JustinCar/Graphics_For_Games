#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	camera = new Camera();
	quad = Mesh::GenerateQuad();
	terrain = Mesh::GenerateFlatTerrain();
	tree = new OBJMesh(MESHDIR "CourseWork/Tree.obj");

	camera->SetPosition(Vector3(1000 * 32 / 2.0f,
		5000.0f, 1000 * 32));

	light = new Light(Vector3((1000 * 32 / 2.0f),
		5000.0f, (1000 * 32 / 2.0f)),
		Vector4(0.9f, 0.9f, 1.0f, 1),
		(1000 * 200));

	reflectShader = new Shader(SHADERDIR "PerPixelVertex.glsl",
		SHADERDIR "reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl",
		SHADERDIR "skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"CourseWork/GrowTerrainVertex.glsl",
		SHADERDIR "CourseWork/TerrainFragment.glsl");
	treeShader = new Shader(SHADERDIR "CourseWork/TreeVertex.glsl",
		SHADERDIR "CourseWork/TreeFragment.glsl");

	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() ||
		!skyboxShader->LinkProgram() || !treeShader->LinkProgram()) {
		return;
	}

	//treeData = new MD5FileData(MESHDIR "CourseWork/Tree.obj");
	//treeNode = new MD5Node(*treeData);

	tree->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/lava.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	std::vector<Mesh*> treeChildren = tree->getChildren();
	for (unsigned int i = 0; i < treeChildren.size(); ++i) {
		treeChildren.at(i)->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/lava.png",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	}
	

	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/lava.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	terrain->SetTexture(SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/stone.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	terrain->SetTexture2(SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/grass.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	terrain->SetTexture3(SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/snow.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	terrain->SetTexture4(SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/water.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	terrain->SetTexture5(SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/lava.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	/*terrainHeightMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/heightMapSmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);*/
	terrainHeightMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/heightMapVSmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	//terrainGrassMap = terrainStoneMap = terrainSnowMap = terrainHeightMap;
		
 	terrainGrassMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/grassmapVsmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	terrainStoneMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/stonemapVsmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	terrainSnowMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/snowmapVsmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	//terrain->SetBumpMap(SOIL_load_OGL_texture(
	//	TEXTUREDIR "Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,
	//	SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR "Coursework/Left_Space.png", TEXTUREDIR "Coursework/Right_Space.png",
		TEXTUREDIR "Coursework/Up_Space.png", TEXTUREDIR "Coursework/Down_Space.png",
		TEXTUREDIR "Coursework/Front_Space.png", TEXTUREDIR "Coursework/Back_Space.png",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);

	if (!cubeMap || !quad->GetTexture() || !terrain->GetTexture()) {
		return;
	}

	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(terrain->GetTexture(), true);
	SetTextureRepeating(terrain->GetTexture2(), true);
	SetTextureRepeating(terrain->GetTexture3(), true);
	SetTextureRepeating(terrain->GetTexture4(), true);
	SetTextureRepeating(terrain->GetTexture5(), true);
	//SetTextureRepeating(heightMap->GetBumpMap(), true);

	init = true;
	waterRotate = 0.0f;

	projMatrix = Matrix4::Perspective(100.0f, 20000.0f,
		(float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

}

Renderer ::~Renderer(void) {
	delete camera;
	delete terrain;
	delete tree;
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	currentShader = 0;
}

void Renderer::reloadShaders() 
{

	reflectShader = new Shader(SHADERDIR "PerPixelVertex.glsl",
		SHADERDIR "reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl",
		SHADERDIR "skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"CourseWork/GrowTerrainVertex.glsl",
		SHADERDIR "CourseWork/TerrainFragment.glsl");
	treeShader = new Shader(SHADERDIR "CourseWork/TreeVertex.glsl",
		SHADERDIR "CourseWork/TreeFragment.glsl");

	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() ||
		!skyboxShader->LinkProgram() || !treeShader->LinkProgram()) {
		return;
	}
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	waterRotate += msec / 30000.0f;
	//treeNode->Update(msec);

}

void Renderer::RenderScene(float msec) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawTree();

	DrawSkybox();

	if (msec > 10)
		DrawTerrain(msec);

	DrawLava();

	SwapBuffers();
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

void Renderer::DrawTree()
{
	SetCurrentShader(treeShader);

	glUniform1i(glGetUniformLocation(currentShader -> GetProgram(),
		"diffuseTex"), 0);	UpdateShaderMatrices();

	tree->Draw();

	glUseProgram(0);
}

void Renderer::DrawTerrain(float msec) {
	SetCurrentShader(lightShader);
	SetShaderLight(*light);


	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)& camera->GetPosition());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrain->GetTexture());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"stoneTex"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrain->GetTexture2());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"grassTex"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, terrain->GetTexture3());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"snowTex"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, terrain->GetTexture4());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"waterTex"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrain->GetTexture5());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"lavaTex"), 4);


	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"time"), msec);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, terrainHeightMap);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "heightMap"), 6);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, terrainGrassMap);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "grassMap"), 7);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, terrainStoneMap);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "stoneMap"), 8);

	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, terrainSnowMap);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "snowMap"), 9);

	//glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
	//	"bumpTex"), 1);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	terrain->Draw();

	glUseProgram(0);
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

	float heightX = (1000 * 32 / 2.0f);

	float heightY = 1000 * 32 / 3.0f;

	float heightZ = (1000 * 32 / 2.0f);

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, 1700, heightZ)) *
		Matrix4::Scale(Vector3(heightX * 2, 1, heightZ * 2)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(500.0f, 500.0f, 500.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
}
