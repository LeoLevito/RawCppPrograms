#pragma once
#include "Shader.h"
//class Shader; //pre-load, pre-empt the shader class or whatever it's called.

class Mesh
{
public:
	Mesh(float* vertices, size_t size); //constructor
	~Mesh(); //destructor
	void Draw(Shader* shader);
private:
	unsigned int VBO, VAO;
};

