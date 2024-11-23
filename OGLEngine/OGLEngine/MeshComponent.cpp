#include "MeshComponent.h"
#include "Cube.h"
#include "Texture.h"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

MeshComponent::MeshComponent()
{
	myShader = new Shader;
	myShader->Initialize("../Shaders/VertexShader.vertexs", "../Shaders/FragmentShader.fragments");
	myTexture = new Texture("../Textures/Bliss2.jpg");
	mesh = new Cube;
	mesh->ApplyTexture(myTexture);

	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

void MeshComponent::DrawComponentSpecificImGuiHierarchyAdjustables(Camera& camera, glm::mat4& projection, Shader& shader)
{
	DrawMesh(camera, projection, shader); //should ideally call this at some other point.
}

void MeshComponent::DrawMesh(Camera& camera, glm::mat4& projection, Shader& shader)
{
	glm::mat4 trans = glm::mat4(1.0f);

	//so how to manipulate these with my transform component?
	trans = glm::translate(trans, position); //translate first so that each object rotates independently.
	trans = glm::rotate(trans, rotation.x, glm::vec3(1, 0, 0));
	trans = glm::rotate(trans, rotation.y, glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, rotation.z, glm::vec3(0, 0, 1));
	trans = glm::scale(trans, scale);

	//write to Vertex Shader
	shader.SetMatrix4(trans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
	shader.SetMatrix4(camera.myView, "view");
	shader.SetMatrix4(projection, "projection");
	mesh->Draw(&shader);
}
