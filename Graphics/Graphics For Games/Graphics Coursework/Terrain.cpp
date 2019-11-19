#include "Terrain.h"

Terrain::Terrain()
{

	stone = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/stone.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	grass = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/grass.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	snow = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/snow.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	water = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/water.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	lava = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/lava.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	terrainHeightMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/heightMapVSmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	terrainGrassMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/grassmapVsmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	terrainStoneMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/stonemapVsmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	terrainSnowMap = SOIL_load_OGL_texture(TEXTUREDIR "Coursework/snowmapVsmall.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	grassBump = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/grassBump.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	stoneBump = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/stoneBump.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	snowBump = SOIL_load_OGL_texture(
		TEXTUREDIR "Coursework/snowBump.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	int VERTICES_SEPARATION = 1;

	numVertices = RAW_WIDTH * RAW_HEIGHT;
	type = GL_TRIANGLE_STRIP;

	numVertices = RAW_WIDTH * RAW_HEIGHT;
	numIndices = (RAW_WIDTH - 1) * (RAW_HEIGHT - 1) * 6;

	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	colours = new Vector4[numVertices];
	normals = new Vector3[numVertices];
	tangents = new Vector3[numVertices];
	indices = new GLuint[numIndices];

	vector<Vector3> positionsForTrees;


	for (int x = 0; x < RAW_WIDTH; ++x) {
		for (int z = 0; z < RAW_HEIGHT; ++z) {
			int offset = (x * RAW_WIDTH) + z;

			vertices[offset] = Vector3(
				x * VERTICES_SEPARATION, 0, z * VERTICES_SEPARATION);

			textureCoords[offset] = Vector2(
				x * TEXTURE_SEPARATION, z * TEXTURE_SEPARATION);

		}
	}

	numIndices = 0;

	for (int x = 0; x < RAW_WIDTH - 1; ++x) {
		for (int z = 0; z < RAW_HEIGHT - 1; ++z) {
			int a = (x * (RAW_WIDTH)) + z;
			int b = ((x + 1) * (RAW_WIDTH)) + z;
			int c = ((x + 1) * (RAW_WIDTH)) + (z + 1);
			int d = (x * (RAW_WIDTH)) + (z + 1);

			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;
		}
	}

	GenerateNormals();
	GenerateTangents();
	BufferData();
}