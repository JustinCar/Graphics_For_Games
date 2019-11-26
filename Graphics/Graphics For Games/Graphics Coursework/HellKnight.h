#pragma once
#include "../../nclgl/Mesh.h"
#include "../../NCLGL/Camera.h"
#include "../../NCLGL/SceneNode.h"
#include "../../nclgl/OBJMesh.h"
#include "../../nclgl/MD5Node.h"

class HellKnight : public SceneNode {
public:
	HellKnight();
	~HellKnight(void) {};

	virtual void Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount);

	void SetShader(Shader* s) { shader = s; };
	void SetLight(Light* l) { light = l; };
	void SetCamera(Camera* c) { camera = c; };

	void SetHellNode(MD5Node* node) { hellNode = node; };

protected:

	Shader* shader;
	Light* light;
	Camera* camera;

	MD5Node* hellNode;

	float coolDown;
	float length;
	bool playing;

	int textureCount;

	GLuint* terrainHeightMap;

	int treePositions[1000][2];
	int treeRotations[1000][1];
	int numTrees;
};

