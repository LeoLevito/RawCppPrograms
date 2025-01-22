#pragma once
#include "Shader.h"
#include "Texture.h"
#include "ObjReader.h"
#include "vboindexer.h"
#include "ShadowMap.h"
//class Shader; //forward declaration.

class Mesh
{
public:
	Mesh(float* vertices, size_t vertexSize, unsigned int* indices, size_t indexSize); //constructor
	Mesh(ObjReader* objreader, const std::string& filename); //constructor 2 for obj
	~Mesh(); //destructor
	void Draw();
	void ApplyDiffuseMap(Texture* texture);
	void ApplySpecularMap(Texture* texture);
	void ApplyShadowMap(ShadowMap& shadowMap);

	std::vector<unsigned int> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

	bool meshLoadedCorrectly;

	void bufferMesh();

	Texture* diffuseMap;
	Texture* specularMap;
	ShadowMap* myShadowMap;

protected:
	unsigned int VBO, VAO, EBO;
	unsigned int NBO, UVBO, CBO;

private:
	int IndicesSize;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};
};

