#pragma once
#include "OGLRenderer.h"

enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER,
	NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER,
	MAX_BUFFER
};

class Mesh {
public:
	Mesh(void);
	~Mesh(void);
	
	virtual void Draw();
	static Mesh* GenerateTriangle();
	static Mesh* GenerateQuad();
	static Mesh* GenerateFlatTerrain();

	void SetTexture(GLuint tex) { texture = tex; }
	GLuint GetTexture() { return texture; }

	void SetTexture2(GLuint tex) { texture2 = tex; }
	GLuint GetTexture2() { return texture2; }

	void SetTexture3(GLuint tex) { texture3 = tex; }
	GLuint GetTexture3() { return texture3; }

	void SetTexture4(GLuint tex) { texture4 = tex; }
	GLuint GetTexture4() { return texture4; }

	void SetTexture5(GLuint tex) { texture5 = tex; }
	GLuint GetTexture5() { return texture5; }

	void SetBumpMap(GLuint tex) { bumpTexture = tex; }
	GLuint GetBumpMap() { return bumpTexture; }
	void GenerateNormals();

	Vector4* getColours() { return colours; }
	Vector3* getVertices() { return vertices; }
	GLuint getNumVertices() { return numVertices; }

protected:
	void BufferData();

	GLuint numIndices;
	unsigned int* indices;
	
	Vector3* normals;


	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;
	
	Vector3* vertices;
	Vector4* colours;

	GLuint texture;
	GLuint texture2;
	GLuint texture3;
	GLuint texture4;
	GLuint texture5;
	Vector2* textureCoords;

	void GenerateTangents();
	Vector3 GenerateTangent(const Vector3& a, const Vector3 & b,
		const Vector3& c, const Vector2& ta,
		const Vector2& tb, const Vector2& tc);

	Vector3* tangents;
	GLuint bumpTexture;
};


