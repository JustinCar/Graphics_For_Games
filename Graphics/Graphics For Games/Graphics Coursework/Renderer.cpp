#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	camera = new Camera();
	quad = Mesh::GenerateQuad();
	terrain = Mesh::GenerateFlatTerrain();

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
		SHADERDIR "PerPixelFragment.glsl");

	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() ||
		!skyboxShader->LinkProgram()) {
		return;
	}

	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/lava.TGA",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	terrain->SetTexture(SOIL_load_OGL_texture(
		TEXTUREDIR "Barren Reds.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	terrainHeightMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/heightMap180Percent8bit.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

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
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	currentShader = 0;
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	waterRotate += msec / 30000.0f;

}

void Renderer::RenderScene(float msec) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

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

void Renderer::DrawTerrain(float msec) {
	SetCurrentShader(lightShader);
	SetShaderLight(*light);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainHeightMap);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"time"), msec);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "heightMap"), 1);

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
		Matrix4::Translation(Vector3(heightX, 2900, heightZ)) *
		Matrix4::Scale(Vector3(heightX * 2, 1, heightZ * 2)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(500.0f, 500.0f, 500.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
}
