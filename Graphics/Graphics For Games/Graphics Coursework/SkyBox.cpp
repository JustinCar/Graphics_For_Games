#include "SkyBox.h"
SkyBox::SkyBox()
{
	camera = 0;
	light = 0;
	shader = 0;

	mesh = Mesh::GenerateQuad();
}

void SkyBox::Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount)
{
	glDepthMask(GL_FALSE);
	r.SetCurrentShader(shader);
	
	r.UpdateShaderMatrices();
	mesh->Draw();
	
	glUseProgram(0);
	glDepthMask(GL_TRUE);
}