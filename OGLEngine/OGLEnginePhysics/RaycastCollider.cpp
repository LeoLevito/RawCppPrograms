#define GLFW_INCLUDE_NONE //fixes issue shit multiple GLAD opengl includes.
#include "RaycastCollider.h"
#include "EditorGUI.h"
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

	trans = glm::translate(trans, glm::vec3(0, 0, 0)); //translate first so that each object rotates independently.
	//trans = glm::rotate(trans, rotation.x, glm::vec3(1, 0, 0));
	//trans = glm::rotate(trans, rotation.y, glm::vec3(0, 1, 0));
	//trans = glm::rotate(trans, rotation.z, glm::vec3(0, 0, 1));
	//trans = glm::scale(trans, scale);

	//write to Vertex Shader
	if (ShaderManager::Get().depthPass == false)
	{
		//use shader for line rendering
		ShaderManager::Get().lineShader->Use();
		ShaderManager::Get().lineShader->SetMatrix4(trans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
		ShaderManager::Get().lineShader->SetMatrix4(Camera::Get().myView, "view");
		ShaderManager::Get().lineShader->SetMatrix4(Camera::Get().projection, "projection");
		//ShaderManager::Get().shader->SetVector3(Camera::Get().myPosition, "viewPos"); //Doesn't really make sense to update this here but whatever.
		glm::vec3 color = { 1,0,0 };
		ShaderManager::Get().lineShader->SetVector3(color, "vertexColor");
	}

	if (drawDebugLine)
	{
		Graphics::Get().DrawLine(startPoint, endPoint);
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

void RaycastCollider::Serialization(std::fstream& file)
{
	file.write(reinterpret_cast<char*>(&drawDebugLine), sizeof(int));
	file.write(reinterpret_cast<char*>(&usePositionAsStartPoint), sizeof(int));
	file.write(reinterpret_cast<char*>(&useDirectionAndDistanceAsEndPoint), sizeof(int));
	file.write(reinterpret_cast<char*>(&useRotationAsDirection), sizeof(int));

	if (!usePositionAsStartPoint)
	{
		file.write(reinterpret_cast<char*>(&startPoint[0]), sizeof(glm::vec3));
	}
	if (!useDirectionAndDistanceAsEndPoint)
	{
		file.write(reinterpret_cast<char*>(&endPoint[0]), sizeof(glm::vec3));
	}
	else
	{
		file.write(reinterpret_cast<char*>(&distance), sizeof(float));
	}
	if (!useRotationAsDirection) 
	{
		file.write(reinterpret_cast<char*>(&direction[0]), sizeof(glm::vec3));
	}
}

void RaycastCollider::Deserialization(std::fstream& file)
{
	file.read(reinterpret_cast<char*>(&drawDebugLine), sizeof(int));
	file.read(reinterpret_cast<char*>(&usePositionAsStartPoint), sizeof(int));
	file.read(reinterpret_cast<char*>(&useDirectionAndDistanceAsEndPoint), sizeof(int));
	file.read(reinterpret_cast<char*>(&useRotationAsDirection), sizeof(int));

	if (!usePositionAsStartPoint)
	{
		file.read(reinterpret_cast<char*>(&startPoint[0]), sizeof(glm::vec3));
	}
	if (!useDirectionAndDistanceAsEndPoint)
	{
		file.read(reinterpret_cast<char*>(&endPoint[0]), sizeof(glm::vec3));
	}
	else
	{
		file.read(reinterpret_cast<char*>(&distance), sizeof(float));
	}
	if (!useRotationAsDirection) 
	{
		file.read(reinterpret_cast<char*>(&direction[0]), sizeof(glm::vec3));
	}
}
