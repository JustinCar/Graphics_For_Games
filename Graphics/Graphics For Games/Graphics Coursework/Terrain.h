#pragma once
#include "../../nclgl/Mesh.h"

#define RAW_WIDTH 1000
#define RAW_HEIGHT 1000

#define TEXTURE_SEPARATION 1.0f / 32.0f

class Terrain : public Mesh {
public:
	Terrain();
	~Terrain(void) {};

	GLuint GetHeightMap() { return terrainHeightMap; }
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

protected:
	GLuint terrainHeightMap;
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
