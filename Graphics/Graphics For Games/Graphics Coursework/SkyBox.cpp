#include "SkyBox.h"
SkyBox::SkyBox()
{
	camera = 0;
	light = 0;
	shader = 0;
	timeAtBlend = 0;
	blendBoxes = false;

	cubeMap = 0;
	cubeMapFog = 0;

	mesh = Mesh::GenerateQuad();
}

void SkyBox::Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount)
{
	if (drawCount > 1 && !blendBoxes)
	{
		blendBoxes = true;
		timeAtBlend = msec;
	}

	glDepthMask(GL_FALSE);
	r.SetCurrentShader(shader);

	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"cubeTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *cubeMap);

	glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"cubeTexFog"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *cubeMapFog);

	float t = (msec - timeAtBlend) / 1000;
	glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"time"), t);

	glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
		"blendBoxes"), blendBoxes);
	
	r.UpdateShaderMatrices();
	mesh->Draw();
	
	glUseProgram(0);
	glDepthMask(GL_TRUE);
}