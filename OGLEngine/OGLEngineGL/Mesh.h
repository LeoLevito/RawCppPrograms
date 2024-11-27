#pragma once
#include "Shader.h"
#include "Texture.h"
#include "ObjReader.h"
//class Shader; //forward declaration.

class Mesh
{
public:
	Mesh(float* vertices, size_t vertexSize, unsigned int* indices, size_t indexSize); //constructor
	Mesh(ObjReader::ObjData objdata); //constructor 2 for obj
	~Mesh(); //destructor
	void Draw(Shader* shader);
	void ApplyTexture(Texture* texture);
protected:
	unsigned int VBO, VAO, EBO;
	Texture* myTexture;
private:
	int IndicesSize;
};

