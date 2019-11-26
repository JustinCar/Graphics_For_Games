#include "Ocean.h"

Ocean::Ocean()
{
	camera = 0;
	light = 0;
	shader = 0;
	isFoggy = false;

	cubeMap = 0;
	cubeMapFog = 0;

	mesh = Mesh::GenerateTerrain();
	mesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/water.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	mesh->SetBumpMap(SOIL_load_OGL_texture(
		TEXTUREDIR "Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
}

void Ocean::Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount)
{
	if (drawCount > 1 && !isFoggy)
	{
		isFoggy = true;
		elapsedTime = msec;
	}

	if (drawCount <= 1)
	{
		isFoggy = false;
	}

	r.SetCurrentShader(shader);
	r.SetShaderLight(*light);

	r.modelMatrix.ToIdentity();

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

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, mesh->GetBumpMap());
	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"bumpTex"), 4);

	glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"time"), (msec - elapsedTime) / 1000);

	glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"fogTime"), msec - elapsedTime);

	glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"isFoggy"), isFoggy);

	glUniform3fv(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"lightningPos"), 1, (float*)& lightningPos);

	if (lightningPos != Vector3(0, 0, 0))
	{
		glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
			"lightningPLaying"), true);
	}
	else
	{
		glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
			"lightningPLaying"), false);
	}

	float heightX = (1000 / 2.0f);

	float heightY = 60;

	float heightZ = (1000 / 2.0f);

	r.modelMatrix =
		Matrix4::Translation(Vector3(1500, heightY, -500)) *
		Matrix4::Scale(Vector3(2, 1, 2)) *
		Matrix4::Rotation(180, Vector3(0.0f, 0.0f, 1.0f));

	r.textureMatrix = Matrix4::Scale(Vector3(1.0f, 1.0f, 1.0f)) *
		Matrix4::Rotation(r.getWaterRotate(), Vector3(0.0f, 0.0f, 1.0f));

	//Matrix4 tempMatrix = r.shadowMatrix * r.modelMatrix;

	//glUniformMatrix4fv(glGetUniformLocation(r.GetCurrentShader()->GetProgram()
	//	, "shadowMatrix"), 1, false, *&tempMatrix.values);

	r.UpdateShaderMatrices();

	mesh->Draw();

	glUseProgram(0);
}