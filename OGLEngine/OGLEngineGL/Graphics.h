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
#include "GameObjectManager.h"

class Graphics
{
public:
	void Initialize(int width, int height, GameObjectManager* gameObjectManager);
	void Render();
	void Cache();
	bool ShouldClose();
	void ExampleCube();
	void EscapeToCloseWindow();
	GLFWwindow* window;
	Camera* myCamera;

	std::vector<glm::vec3> myCubePositions;
	glm::mat4 projection;
	Shader* myShader;
private:
	unsigned int VBO; //vertex buffer object, OpenGL objects are unsigned ints. We can put a bunch of vertices in this object and send it to the GPU. You can have an array of VBOs.
	unsigned int VAO;


	Triangle* myTriangle;
	Cube* myCube;
	Texture* myTexture;


	GameObjectManager* myGameObjectManager;

	float myWidth;
	float myHeight;
};

