#include "Lightning.h"
Lightning::Lightning()
{
	camera = 0;
	light = 0;
	shader = 0;
	fog = false;
	coolDown = 1.0f;
	length = 3.3f;
	playing = false;
	textureCount = 0;

	mesh = Mesh::GenerateQuad();

	mesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/Lightning1.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	/*mesh->SetTexture2(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/Lightning2.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	mesh->SetTexture2(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/Lightning3.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));*/
}

void Lightning::Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount)
{
	//if (drawCount < 3)
	//{
	//	elapsedTime = 0;
	//	fog = false;
	//	return;
	//}

	//if ((msec - elapsedTime) / 1000 >= coolDown)
	//{
	//	if (!playing) 
	//	{
	//		playing = true;
	//		elapsedTime = msec;
	//		//mesh->repositionPoints();
	//	}
	//}
	//else
	//	return;

	//if (msec - elapsedTime == 0)
	//{
	//	return;
	//}

	//if ((msec - elapsedTime) / 1000 >= length)
	//{
	//	playing = false;
	//	elapsedTime = msec;
	//	textureCount++;
	//	if (textureCount > 2)
	//		textureCount = 0;
	//	return;
	//}
	
	r.SetCurrentShader(shader);

	r.modelMatrix.ToIdentity();
	r.textureMatrix.ToIdentity();

	glUniform3fv(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"cameraPos"), 1, (float*)& camera->GetPosition());

	Vector3 center = Vector3(0.0f, 0.0f, 0.0f);
	glUniform3fv(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"centerPos"), 1, (float*)& center);

	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"diffuseTex"), 0);

	glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"time"), msec - elapsedTime);

	float heightX = (1000 / 2.0f);

	float heightY = 60;

	float heightZ = (1000 / 2.0f);

	Vector3 currentDirection = Vector3(1, 0, 0);
	Vector3 direction = camera->GetPosition() - Vector3(0, camera->GetPosition().y, 0);
	direction.Normalise();

	float rotationAngle = (float)acos(currentDirection.Dot(currentDirection, direction));


	
	r.modelMatrix =
		Matrix4::Translation(Vector3(0, 1050, 0)) *
		Matrix4::Scale(Vector3(300, 1000, 100)) *
		Matrix4::Rotation((rotationAngle * 360), Vector3(0.0f, 1.0f, 0.0f));	
	r.UpdateShaderMatrices();

	mesh->Draw();

	glUseProgram(0);

}