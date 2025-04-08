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
private:
	Graphics();
	~Graphics();
public:
	static Graphics& Get();
	void Initialize(int width, int height);
	void Render();
	bool ShouldClose();
	void ExampleCube();
	void EscapeToCloseWindow();
	void RenderToSceneTexture(unsigned int FBO, unsigned int texture);
	void RenderWorldGrid();
	void DrawLine(glm::vec3 startPoint, glm::vec3 endPoint);
	void DrawImgui();
	void RenderPickingPass();

	GLFWwindow* window;

	std::vector<glm::vec3> myCubePositions;
	Shader* myShader;

	unsigned int sceneFBO;
	unsigned int sceneTexture;
	unsigned int rbo;

	float myWidth;
	float myHeight;

	glm::vec3 SceneBackgroundColor{0.1f, 0.1f, 0.1f};

	unsigned int pickingFBO;
	unsigned int sceneTexturePicking;
private:
	unsigned int VAO;
	unsigned int VBO;//vertex buffer object, OpenGL objects are unsigned ints. We can put a bunch of vertices in this object and send it to the GPU. You can have an array of VBOs.
	unsigned int EBO;



	Triangle* myTriangle;
	Cube* myCube;
	Texture* myTexture;


};

