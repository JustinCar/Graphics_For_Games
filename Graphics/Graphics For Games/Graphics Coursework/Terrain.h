#pragma once
#include "../../nclgl/Mesh.h"
#include "../../NCLGL/Camera.h"
#include "../../NCLGL/SceneNode.h"

class Terrain : public SceneNode {
public:
	Terrain();
	~Terrain(void) {};

	virtual void Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount);

	GLuint* GetHeightMap() { return terrainHeightMap; }
	GLuint GetGrassMap() { return terrainGrassMap; }
	GLuint GetStoneMap() { return terrainStoneMap; }
	GLuint GetSnowMap() { return terrainSnowMap; }
	GLuint GetStone() { return stone; }
	GLuint GetGrass() { return grass; }
	GLuint GetSnow() { return snow; }
	GLuint GetWater() { return water; }
	GLuint GetLava() { return lava; }
	GLuint GetGrassBump() { return grassBump; }
	GLuint GetStoneBump() { return stoneBump; }
	GLuint GetSnowBump() { return snowBump; }

	void SetShader(Shader* shader) { lightShader = shader; };
	void SetLight(Light* l) { light = l; };
	void SetCamera(Camera* c) { camera = c; };
	void SetHeightMap(GLuint* m) { terrainHeightMap = m; };

protected:

	Shader* lightShader;
	Light* light;
	Camera* camera;

	bool fog;

	float fogElapsedTime;

	GLuint* terrainHeightMap;
	GLuint terrainGrassMap;
	GLuint terrainStoneMap;
	GLuint terrainSnowMap;

	GLuint stone;
	GLuint grass;
	GLuint snow;
	GLuint water;
	GLuint lava;

	GLuint grassBump;
	GLuint stoneBump;
	GLuint snowBump;
};
