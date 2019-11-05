# pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclGL/Camera.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);
	
	virtual void RenderScene();
	virtual void UpdateScene(float msec);
	
	void UpdateTextureMatrix(float rotation);
	void ToggleRepeating();
	void ToggleFiltering();
	
protected:
	Mesh* triangle;
	Camera* camera;
	
	bool filtering;
	bool repeating;};