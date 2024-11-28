#pragma once
#include "Shader.h"
#include "Texture.h"
#include "ObjReader.h"
#include "vboindexer.h"
//class Shader; //forward declaration.

class Mesh
{
public:
	Mesh(float* vertices, size_t vertexSize, unsigned int* indices, size_t indexSize); //constructor
	Mesh(ObjReader* objreader, vboindexer* VBOindexer); //constructor 2 for obj
	~Mesh(); //destructor
	void Draw(Shader* shader);
	void ApplyTexture(Texture* texture);
protected:
	unsigned int VBO, VAO, EBO;
	unsigned int NBO, UVBO, CBO;
	Texture* myTexture;
private:
	int IndicesSize;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};
};

