#pragma once
#include "../../nclgl/Mesh.h"
#include "../../NCLGL/Camera.h"
#include "../../NCLGL/SceneNode.h"
#include "../../nclgl/OBJMesh.h"

class Ocean : public SceneNode {
public:
	Ocean();
	~Ocean(void) {};

	virtual void Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount);

	void SetShader(Shader* s) { shader = s; };
	void SetLight(Light* l) { light = l; };
	void SetCamera(Camera* c) { camera = c; };
	void SetCubeMap(GLuint* c) { cubeMap = c; };
	void SetCubeMapFog(GLuint* c) { cubeMapFog = c; };

protected:

	GLuint* cubeMap;
	GLuint* cubeMapFog;

	Shader* shader;
	Light* light;
	Camera* camera;
};
