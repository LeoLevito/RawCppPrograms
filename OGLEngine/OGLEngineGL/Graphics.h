#pragma once
#include <GLAD/glad.h> //bruh.
#include <glfw3.h>
#include "Shader.h"

class Graphics
{
public:
	void Initialize(int width, int height);
	void Render();
	void Cache();
	bool ShouldClose();
private:
	unsigned int VBO; //vertex buffer object, OpenGL objects are unsigned ints. We can put a bunch of vertices in this object and send it to the GPU.
	//example of multiple buffer objects:
		//unsigned int VBOs[5]; 
		//glGenBuffers(5, &VBO);
	
	unsigned int VAO;
	GLFWwindow* window;
	Shader myShader;
};

