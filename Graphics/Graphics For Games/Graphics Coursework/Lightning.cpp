#include "Lightning.h"
// Lightning texture:
// <a href="https://www.freepik.com/free-photos-vectors/background">Background vector created by pikisuperstar - www.freepik.com</a>
Lightning::Lightning()
{
	camera = 0;
	light = 0;
	shader = 0;
	coolDown = 1.1f;
	length = 1.2f;
	playing = false;
	textureCount = 0;

	position.ToZero();

	mesh = Mesh::GenerateLightningPoint();

	mesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/LightningTest.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	mesh->SetTexture2(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/LightningTest2.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
}

void Lightning::Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount)
{
	if (drawCount < 4)
	{
		elapsedTime = 0;
		return;
	}

	if ((msec - elapsedTime) / 1000 >= coolDown)
	{
		if (!playing) 
		{
			playing = true;
			elapsedTime = msec;

			mesh->repositionPoint();
			position = mesh->getVertices()[0];

		}
	}
	else
		return;

	if (msec - elapsedTime == 0)
	{
		return;
	}

	if ((msec - elapsedTime) / 1000 >= length)
	{
		playing = false;
		position.ToZero();
		terrain->setLightningPosition(position);
		tree->setLightningPosition(position);
		ocean->setLightningPosition(position);
		elapsedTime = msec;
		textureCount++;
		if (textureCount > 1)
			textureCount = 0;
		return;
	}
	
	r.SetCurrentShader(shader);

	r.modelMatrix.ToIdentity();
	r.textureMatrix.ToIdentity();

	// Billboarding
	Vector3 camPos = camera->GetPosition();
	Vector3 lightningPos = mesh->getVertices()[0];
	camPos.y = lightningPos.y;
	Vector3 f = mesh->getVertices()[0] - camPos;
	f.Normalise();
	Vector3 R = Vector3::Cross(Vector3(0, 1, 0), f);
	Vector3 u = Vector3::Cross(f, R);

	glUniform3fv(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"cameraPos"), 1, (float*)& camera->GetPosition());

	glUniform3fv(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"up"), 1, (float*)& u);

	glUniform3fv(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"right"), 1, (float*)& R);

	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"diffuseTex2"), 1);

	glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"time"), msec - elapsedTime);

	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"texCount"), textureCount);
	r.UpdateShaderMatrices();

	mesh->Draw();

	terrain->setLightningPosition(position);
	tree->setLightningPosition(position);
	ocean->setLightningPosition(position);

	glUseProgram(0);

}