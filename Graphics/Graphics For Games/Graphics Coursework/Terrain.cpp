#include "Terrain.h"

Terrain::Terrain()
{
	camera = 0;
	light = 0;
	lightShader = 0;

	stone = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/stone.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	grass = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/grass.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	snow = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/snow.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	water = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/water.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	lava = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/lava.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	terrainGrassMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/grassmapVsmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	terrainStoneMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/stonemapVsmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	terrainSnowMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/snowmapVsmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	grassBump = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/grassBump.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	stoneBump = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/stoneBump.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	snowBump = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/snowBump.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
}

void Terrain::Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount) {

	if (drawCount < 1)
		return;

	r.SetCurrentShader(lightShader);
	r.SetShaderLight(*light);

	r.modelMatrix.ToIdentity();

	r.UpdateShaderMatrices();


	Matrix4 tempMatrix = r.shadowMatrix * r.modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(r.GetCurrentShader()->GetProgram()
		, "shadowMatrix"), 1, false, *&tempMatrix.values);


	glUniform3fv(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"cameraPos"), 1, (float*)& camera->GetPosition());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, stone);
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"stoneTex"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grass);
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"grassTex"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, snow);
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"snowTex"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, water);
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"waterTex"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, lava);
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"lavaTex"), 4);


	glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"time"), msec);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, *terrainHeightMap);
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(), "heightMap"), 6);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, terrainGrassMap);
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(), "grassMap"), 7);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, terrainStoneMap);
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(), "stoneMap"), 8);

	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, terrainSnowMap);
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(), "snowMap"), 9);


	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, grassBump);
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"grassBump"), 10);

	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"shadowTex"), 11);

	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, stoneBump);
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"stoneBump"), 12);

	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, snowBump);
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"snowBump"), 13);

	mesh->Draw();
	glUseProgram(0);
}