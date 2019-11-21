#include "Rain.h"

Rain::Rain()
{
	camera = 0;
	light = 0;
	shader = 0;

	mesh = Mesh::GeneratePoints(100000);
	mesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/water.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
}

void Rain::Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount)
{
	if (drawCount <= 2)
		return;

	r.SetCurrentShader(shader);
	r.SetShaderLight(*light);

	r.modelMatrix.ToIdentity();
	r.textureMatrix.ToIdentity();

	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"diffuseTex"), 0);

	glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"time"), msec);

	r.UpdateShaderMatrices();

	mesh->Draw();

	glUseProgram(0);
}