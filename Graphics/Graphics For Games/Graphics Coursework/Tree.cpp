#include "Tree.h"
Tree::Tree()
{
	camera = 0;
	light = 0;
	shader = 0;
	fog = false;

	terrainHeightMap = 0;

	numTrees = 1000;
	tree = new OBJMesh(MESHDIR "CourseWork/TreeHD.obj");
	mesh = tree;

	for (int i = 0; i < numTrees; i++)
	{
		treePositions[i][0] = rand() % 800 + 100;
		treePositions[i][1] = rand() % 800 + 100;
		treeRotations[i][0] = rand() % 360 + 1;
	}

	tree->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/Forest/bark01_bottom.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	tree->GenerateNormals();

	std::vector<Mesh*> treeChildren = tree->getChildren();
	treeChildren[0]->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "Coursework/Forest/tree_branches.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	treeChildren[0]->GenerateNormals();

	tree->SetBumpMap(SOIL_load_OGL_texture(
		TEXTUREDIR "Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	treeChildren[0]->SetBumpMap(SOIL_load_OGL_texture(
		TEXTUREDIR "Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
}


void Tree::Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount)
{
	if (drawCount < 1)
		return;

	if (drawCount <= 1)
		fog = false;

	if (drawCount > 1 && !fog)
	{
		fog = true;
		elapsedTime = msec;
	}

	for (int i = 0; i < numTrees; i++)
	{
		r.SetCurrentShader(shader);
		r.SetShaderLight(*light);

		r.modelMatrix.ToIdentity();
		r.textureMatrix.ToIdentity();

		glUniform3fv(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
			"cameraPos"), 1, (float*)& camera->GetPosition());

		glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
			"diffuseTex"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tree->GetBumpMap());
		glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
			"bumpTex"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, *terrainHeightMap);
		glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(), "heightMap"), 2);

		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, shadowTex);
		glUniform1i(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
			"shadowTex"), 11);

		glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
			"time"), msec);

		glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
			"fogTime"), msec - elapsedTime);

		glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
			"isFoggy"), fog);

		glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
			"xPos"), treePositions[i][0] * TEXTURE_SEPARATION);

		glUniform1f(glGetUniformLocation(r.GetCurrentShader()->GetProgram(),
			"zPos"), treePositions[i][1] * TEXTURE_SEPARATION);

		r.modelMatrix = Matrix4::Translation(Vector3(treePositions[i][0], 0, treePositions[i][1])) * Matrix4::Scale(Vector3(1, 1, 1))
			* Matrix4::Rotation(treeRotations[i][0], Vector3(0.0f, 1.0f, 0.0f));;

		Matrix4 tempMatrix = r.shadowMatrix * r.modelMatrix;

		glUniformMatrix4fv(glGetUniformLocation(r.GetCurrentShader()->GetProgram()
			, "shadowMatrix"), 1, false, *&tempMatrix.values);

		r.UpdateShaderMatrices();

		mesh->Draw();

		glUseProgram(0);
	}
}