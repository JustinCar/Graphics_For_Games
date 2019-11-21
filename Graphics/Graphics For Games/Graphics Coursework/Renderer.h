#pragma once

#include "../../NCLGL/OGLRenderer.h"
#include "../../NCLGL/Camera.h"
#include "../../NCLGL/HeightMap.h"
#include "../../NCLGL/MD5Mesh.h"
#include "../../nclgl/MD5Node.h"
#include "../../nclgl/OBJMesh.h"
#include "../../nclgl/SceneNode.h"
#include "Terrain.h"
#include <stdlib.h>
#include "Tree.h"
#include "Ocean.h"
#include "SkyBox.h"
#include "Rain.h"

#define SHADOWSIZE 2048

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene(float msec);
	virtual void UpdateScene(float msec);

	void reloadShaders();


protected:

	void DrawShadowScene(float msec);
	void DrawCombinedScene(float msec);

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* treeShader;
	Shader* shadowShader;
	Shader* rainShader;

	int treePositions[1000][2];
	int numTrees;
	Mesh* quad;
	Mesh* terrain;
	GLuint terrainHeightMap;

	SkyBox* skyBoxNode;
	Terrain* terrainNode;
	Tree* treeNode;
	Ocean* oceanNode;
	Rain* rainNode;

	int  drawCount;
	float elapsedTime;

	SceneNode* root;

	Light* light;
	Camera* camera;

	GLuint cubeMap;
	GLuint cubeMapFog;

	GLuint shadowTex;
	GLuint shadowFBO;

	bool started;
};


