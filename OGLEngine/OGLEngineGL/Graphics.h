#pragma once
#include <GLAD/glad.h> //bruh.
#include <glfw3.h>
#include "Shader.h"
#include "Triangle.h"
#include "Cube.h"
#include <glm.hpp>
#include <vector>
#include "Camera.h"

class Graphics
{
public:
	void Initialize(int width, int height);
	void Render();
	void Cache();
	bool ShouldClose();
	void ExampleCube();
	void EscapeToCloseWindow();
	GLFWwindow* window;
	Camera* myCamera;
private:
	unsigned int VBO; //vertex buffer object, OpenGL objects are unsigned ints. We can put a bunch of vertices in this object and send it to the GPU.
	//example of multiple buffer objects:
		//unsigned int VBOs[5]; 
		//glGenBuffers(5, &VBO);
	
	unsigned int VAO;

	Shader* myShader;
	Triangle* myTriangle;
	Cube* myCube;
	std::vector<glm::vec3> myCubePositions;
	glm::mat4 projection;
	float myWidth;
	float myHeight;
};

