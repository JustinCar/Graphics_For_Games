#pragma once
#include "../../nclgl/Mesh.h"
#include "../../NCLGL/Camera.h"
#include "../../NCLGL/SceneNode.h"
#include "../../nclgl/OBJMesh.h"
#include <cmath>

class Lightning : public SceneNode {
public:
	Lightning();
	~Lightning(void) {};

	virtual void Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount);

	void SetShader(Shader* s) { shader = s; };
	void SetLight(Light* l) { light = l; };
	void SetCamera(Camera* c) { camera = c; };

protected:

	OBJMesh* tree;

	Shader* shader;
	Light* light;
	Camera* camera;

	bool fog;
	float coolDown;
	float length;
	bool playing;

	int textureCount;

	GLuint* terrainHeightMap;

	int treePositions[1000][2];
	int treeRotations[1000][1];
	int numTrees;
};
