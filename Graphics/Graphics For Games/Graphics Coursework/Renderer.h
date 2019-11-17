#pragma once

#include "../../NCLGL/OGLRenderer.h"
#include "../../NCLGL/Camera.h"
#include "../../NCLGL/HeightMap.h"
#include "../../NCLGL/MD5Mesh.h"
#include "../../nclgl/MD5Node.h"
#include "../../nclgl/OBJMesh.h"
#include "../../nclgl/SceneNode.h"

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
	void DrawTree(float msec);

	void DrawShadowScene(float msec);
	void DrawCombinedScene(float msec);

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* treeShader;
	Shader* shadowShader;

	OBJMesh* tree;
	//OBJMesh* Rock1;
	Mesh* quad;
	Mesh* terrain;
	GLuint terrainHeightMap;
	GLuint terrainGrassMap;
	GLuint terrainStoneMap;
	GLuint terrainSnowMap;

	Light* light;
	Camera* camera;

	GLuint cubeMap;

	GLuint shadowTex;
	GLuint shadowFBO;

	float waterRotate;
};


