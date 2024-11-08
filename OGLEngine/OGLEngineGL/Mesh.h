#pragma once
#include "Shader.h"
//class Shader; //forward declaration.

class Mesh
{
public:
	Mesh(float* vertices, size_t vertexSize, unsigned int* indices, size_t indexSize); //constructor
	~Mesh(); //destructor
	void Draw(Shader* shader);
protected:
	unsigned int VBO, VAO, EBO;
};

