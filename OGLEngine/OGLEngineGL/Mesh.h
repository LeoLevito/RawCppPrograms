#pragma once
#include "Shader.h"
//class Shader; //pre-load, pre-empt the shader class or whatever it's called.

class Mesh
{
public:
	Mesh(float* vertices, size_t vertexSize, unsigned int* indices, size_t indexSize); //constructor
	~Mesh(); //destructor
	void Draw(Shader* shader);
private:
	unsigned int VBO, VAO, EBO;
};

