#pragma once
#include "../../nclgl/Mesh.h"
#include "../../NCLGL/Camera.h"
#include "../../NCLGL/SceneNode.h"
#include "../../nclgl/OBJMesh.h"
#include <cmath>
#include "Terrain.h"
#include "Ocean.h"
#include "Tree.h"

class Lightning : public SceneNode {
public:
	Lightning();
	~Lightning(void) {};

	virtual void Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount);

	void SetShader(Shader* s) { shader = s; };
	void SetLight(Light* l) { light = l; };
	void SetCamera(Camera* c) { camera = c; };
	void SetTerrain(Terrain* t) { terrain = t; };
	void SetOcean(Ocean* o) { ocean = o; };
	void SetTree(Tree* t) { tree = t; };

	float getXPos() { return heightX; };
	float getZPos() { return heightZ; };

protected:

	Shader* shader;
	Light* light;
	Camera* camera;

	Terrain* terrain;
	Ocean* ocean;
	Tree* tree;

	float coolDown;
	float length;
	bool playing;

	float heightX;
	float heightZ;

	GLuint tex1;
	GLuint tex2;

	Vector3 position;

	int textureCount;

	GLuint* terrainHeightMap;

	int treePositions[1000][2];
	int treeRotations[1000][1];
	int numTrees;
};
