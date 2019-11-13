# pragma once

#include "../../NCLGL/OGLRenderer.h"
#include "../../NCLGL/Camera.h"
#include "../../NCLGL/HeightMap.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window & parent);
	virtual ~Renderer(void);
	
	virtual void RenderScene();
	virtual void UpdateScene(float msec);
	
protected:
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();
	
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	
	HeightMap* heightMap;
	Mesh* quad;
	
	Light* light;
	Camera* camera;
	
	GLuint cubeMap;
	
	float waterRotate;
};


