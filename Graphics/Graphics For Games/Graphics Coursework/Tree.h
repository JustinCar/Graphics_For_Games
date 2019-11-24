#pragma once
#include "../../nclgl/Mesh.h"
#include "../../NCLGL/Camera.h"
#include "../../NCLGL/SceneNode.h"
#include "../../nclgl/OBJMesh.h"

class Tree : public SceneNode {
public:
	Tree();
	~Tree(void) {};

	virtual void Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount);

	void SetShader(Shader* s) { shader = s; };
	void SetLight(Light* l) { light = l; };
	void SetCamera(Camera* c) { camera = c; };

	OBJMesh* GetOBJ() { return tree; };

	void SetHeightMap(GLuint* m) { terrainHeightMap = m; };

protected:

	OBJMesh* tree;

	Shader* shader;
	Light* light;
	Camera* camera;

	bool fog;

	float fogElapsedTime;

	GLuint* terrainHeightMap;

	int treePositions[1000][2];
	int treeRotations[1000][1];
	int numTrees;
};
