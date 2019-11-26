#pragma once
#include "../../nclgl/Mesh.h"
#include "../../NCLGL/Camera.h"
#include "../../NCLGL/SceneNode.h"
#include "../../nclgl/OBJMesh.h"

// Sky box:
// https://assetstore.unity.com/packages/2d/textures-materials/sky/allsky-free-10-sky-skybox-set-146014
class SkyBox : public SceneNode {
public:
	SkyBox();
	~SkyBox(void) {};

	virtual void Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount);

	void SetShader(Shader* s) { shader = s; };
	void SetLight(Light* l) { light = l; };
	void SetCamera(Camera* c) { camera = c; };
	void SetCubeMap(GLuint* c) { cubeMap = c; };
	void SetCubeMapFog(GLuint* c) { cubeMapFog = c; };

protected:

	GLuint* cubeMap;
	GLuint* cubeMapFog;

	float timeAtBlend;
	bool blendBoxes;

	Shader* shader;
	Light* light;
	Camera* camera;
};
