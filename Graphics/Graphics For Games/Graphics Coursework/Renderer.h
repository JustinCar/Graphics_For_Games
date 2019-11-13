#pragma once

#include "../../NCLGL/OGLRenderer.h"
#include "../../NCLGL/Camera.h"
#include "../../NCLGL/HeightMap.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene(float msec);
	virtual void UpdateScene(float msec);

protected:
	void DrawTerrain(float msec);
	void DrawLava();
	void DrawSkybox();

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;

	Mesh* quad;
	Mesh* terrain;
	GLuint terrainHeightMap;

	Light* light;
	Camera* camera;

	GLuint cubeMap;



	float waterRotate;
};


