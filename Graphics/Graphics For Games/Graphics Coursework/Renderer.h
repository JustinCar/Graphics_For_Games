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

#define SHADOWSIZE 2048

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene(float msec);
	virtual void UpdateScene(float msec);

	void reloadShaders();

protected:
	void DrawTerrain(float msec);
	void DrawLava();
	void DrawSkybox();
	void DrawTree(float msec, int index);

	void DrawShadowScene(float msec);
	void DrawCombinedScene(float msec);

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* treeShader;
	Shader* shadowShader;

	int treePositions[1000][2];
	int numTrees;
	//int treeSizes[1000][1];
	//OBJMesh* Rock1;
	Mesh* quad;
	Mesh* terrain;
	GLuint terrainHeightMap;
	GLuint terrainGrassMap;
	GLuint terrainStoneMap;
	GLuint terrainSnowMap;

	int  drawCount;

	SceneNode* root;

	Light* light;
	Camera* camera;

	GLuint cubeMap;

	GLuint shadowTex;
	GLuint shadowFBO;

	bool started;
	float timeAtStart;

};


