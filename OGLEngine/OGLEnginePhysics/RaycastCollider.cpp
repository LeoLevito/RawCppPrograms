#define GLFW_INCLUDE_NONE //fixes issue shit multiple GLAD opengl includes.
#include "RaycastCollider.h"
#include <GLFW/glfw3.h>
#include <GLAD/glad.h>
#include "ShaderManager.h"
#include <gtc/matrix_transform.hpp>


RaycastCollider::RaycastCollider()
{
	//we only wanna do glGen once.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); 
	glGenBuffers(1, &EBO);
}

RaycastCollider::~RaycastCollider()
{
}

void RaycastCollider::SetPosition(glm::vec3 pos)
{
	position = pos;
	if (usePositionAsStartPoint)
	{
		startPoint = position; 
	}
}

void RaycastCollider::SetRotation(glm::vec3 rot)
{
	rotation = rot;
	if (useRotationAsDirection)
	{
		direction = rotation;
	}
}

void RaycastCollider::SetScale(glm::vec3 scaleIn)
{
	scale = scaleIn;
}

void RaycastCollider::Update()
{

	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, glm::vec3(0,0,0)); //translate first so that each object rotates independently.
	//trans = glm::rotate(trans, rotation.x, glm::vec3(1, 0, 0));
	//trans = glm::rotate(trans, rotation.y, glm::vec3(0, 1, 0));
	//trans = glm::rotate(trans, rotation.z, glm::vec3(0, 0, 1));
	//trans = glm::scale(trans, scale);

	//write to Vertex Shader
	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->SetMatrix4(trans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
		ShaderManager::Get().shader->SetMatrix4(Camera::Get().myView, "view");
		ShaderManager::Get().shader->SetMatrix4(Camera::Get().projection, "projection");
		ShaderManager::Get().shader->SetVector3(Camera::Get().myPosition, "viewPos"); //Doesn't really make sense to update this here but whatever.
	}

	if (drawDebugLine)
	{
		glm::vec3 vert1 = startPoint;
		glm::vec3 vert2 = endPoint;

		std::vector<glm::vec3> verts = { vert1, vert2 };
		std::vector<unsigned int> indices = { 0 , 1 };

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); //the indices might be wrong

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //these are probably not right, need to modify to read correctly.

		glBindVertexArray(0);




		glBindVertexArray(VAO);
		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0); //cast?
		glBindVertexArray(0);

	}
}

void RaycastCollider::DrawImgui()
{
	//whether to use position as start point.
	if (ImGui::Checkbox("Use position as start point", &usePositionAsStartPoint))
	{
	}
	//whether to use direction and distance as end point.
	if (ImGui::Checkbox("Use direction + distance as end point", &useDirectionAndDistanceAsEndPoint))
	{
	}
	if (useDirectionAndDistanceAsEndPoint)
	{
		if (ImGui::Checkbox("Use rotation as direction (BUGGED)", &useRotationAsDirection)) //rotation is bugged, my Calculate...() function won't angle it correctly, and won't show anything if rotation is 0,0,0.
		{
		}
		if (ImGui::DragFloat("Specify distance", &distance, .01f))
		{
		}
		if (!useRotationAsDirection)
		{
			if (ImGui::DragFloat3("Specify direction", &direction.x, .01f));
		}
		CalculateStartAndEndPoints(); //Issue with this is that it only updates if we have the collider component open in the hierarchy ImGui window. Should just move this to update.
	}
	if (!usePositionAsStartPoint)
	{
		if (ImGui::DragFloat3("Specify start point manually", &startPoint.x, .01f));
	}
	if (!useDirectionAndDistanceAsEndPoint)
	{
		if (ImGui::DragFloat3("Specify end point manually", &endPoint.x, .01f));
	}

	if (ImGui::Checkbox("Draw Debug Line", &drawDebugLine))
	{
	}
	
}

void RaycastCollider::CalculateStartAndEndPoints()
{
	//basic start and end point.


	//direction, distance
	endPoint = startPoint + (glm::normalize(direction) * distance);
}
