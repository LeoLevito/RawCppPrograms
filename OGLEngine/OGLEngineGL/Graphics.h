#pragma once
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Triangle.h"
#include "Cube.h"
#include <glm.hpp>
#include <vector>
#include "Camera.h"
#include "Texture.h"

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

	std::vector<glm::vec3> myCubePositions;
private:
	unsigned int VBO; //vertex buffer object, OpenGL objects are unsigned ints. We can put a bunch of vertices in this object and send it to the GPU. You can have an array of VBOs.
	unsigned int VAO;

	Shader* myShader;
	Triangle* myTriangle;
	Cube* myCube;
	Texture* myTexture;


	glm::mat4 projection;

	float myWidth;
	float myHeight;
};

