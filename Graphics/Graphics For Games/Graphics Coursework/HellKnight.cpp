#include "HellKnight.h"
HellKnight::HellKnight()
{
	camera = 0;
	light = 0;
	shader = 0;
	coolDown = 1.0f;
	length = 3.3f;
	playing = false;
	textureCount = 0;
}

void HellKnight::Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount)
{

	if (drawCount < 4)
		return;

	glEnable(GL_CULL_FACE);
	r.SetCurrentShader(shader);

	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(), "diffuseTex"), 0);

	r.modelMatrix = Matrix4::Scale(Vector3(0.2, 0.2, 0.2)) * Matrix4::Translation(Vector3(2500, 350, 2500));

	r.UpdateShaderMatrices();

	hellNode->Draw(r);

	glUseProgram(0);
	glDisable(GL_CULL_FACE);


}