#include "Ocean.h"

Ocean::Ocean()
{
	camera = 0;
	light = 0;
	shader = 0;
	isFoggy = false;

	cubeMap = 0;
	cubeMapFog = 0;

	mesh = Mesh::GenerateQuad();
	mesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/water.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
}

void Ocean::Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount)
{
	if (drawCount > 1)
		isFoggy = true;

	if (drawCount <= 1)
		isFoggy = false;

	r.SetCurrentShader(shader);
	r.SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"cameraPos"), 1, (float*)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"cubeTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *cubeMap);

	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"cubeTexFog"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *cubeMapFog);

	glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"time"), msec / 1000);

	glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"isFoggy"), isFoggy);

	float heightX = (1000 / 2.0f);

	float heightY = 60;

	float heightZ = (1000 / 2.0f);

	r.modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX * 2, 1, heightZ * 2)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	r.textureMatrix = Matrix4::Scale(Vector3(50.0f, 50.0f, 50.0f)) *
		Matrix4::Rotation(r.getWaterRotate(), Vector3(0.0f, 0.0f, 1.0f));

	r.UpdateShaderMatrices();

	mesh->Draw();

	glUseProgram(0);
}