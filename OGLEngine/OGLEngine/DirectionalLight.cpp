#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
}

void DirectionalLight::UpdateIDBasedStrings()
{
	IDstring = "directionalLights[";
	IDstring += std::to_string(ID);
	IDstring += "].";

	positionString = IDstring;
	positionString += "position";

	directionString = IDstring;
	directionString += "direction";

	ambientString = IDstring;
	ambientString += "ambient";

	diffuseString = IDstring;
	diffuseString += "diffuse";

	specularString = IDstring;
	specularString += "specular";
}

void DirectionalLight::SetToDefault()
{
	ambient = glm::vec3(0, 0, 0);
	diffuse = glm::vec3(1, 1, 1);
	specular = glm::vec3(1, 1, 1);

	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->SetVector3(ambient, ambientString);
		ShaderManager::Get().shader->SetVector3(diffuse, diffuseString);
		ShaderManager::Get().shader->SetVector3(specular, specularString);
	}
}

void DirectionalLight::SetToCurrent()
{
	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->SetVector3(ambient, ambientString);
		ShaderManager::Get().shader->SetVector3(diffuse, diffuseString);
		ShaderManager::Get().shader->SetVector3(specular, specularString);
	}
}

void DirectionalLight::SetToZero()
{
	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), ambientString);
		ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), diffuseString);
		ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), specularString);
	}
}

void DirectionalLight::SetPosition(glm::vec3 pos)
{
	position = pos;
	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->SetVector3(position, positionString);
	}
}

void DirectionalLight::SetDirection(glm::vec3 dir)
{
	direction = dir;
	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->SetVector3(direction, directionString);
	}

	glm::mat4 lightProjection;
	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f;
	float far_plane = 100.f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::vec3 worldup = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(worldup, direction));
	glm::vec3 up = glm::cross(direction, right);
	glm::vec3 normalizedup = glm::normalize(up);
	lightView = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //should probably put this in light component.
	lightSpaceMatrix = lightProjection * lightView;
	//ShaderManager::Get().depthShader->Use();

	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->SetMatrix4(lightSpaceMatrix, "lightSpaceMatrix");
	}
	else
	{
		ShaderManager::Get().depthShader->SetMatrix4(lightSpaceMatrix, "lightSpaceMatrix");
	}
}

void DirectionalLight::DrawImgui()
{
	Light::DrawImgui();

	if (ImGui::Button("reset to defaults"))
	{
		SetToDefault();
	}
	if (ImGui::Button("reset to zero"))
	{
		SetToZero();
	}

	if (ImGui::DragFloat3("ambient", &ambient.x, .01f))
	{
		if (ShaderManager::Get().depthPass == false)
		{
			ShaderManager::Get().shader->SetVector3(ambient, ambientString);
		}
	}
	if (ImGui::DragFloat3("diffuse", &diffuse.x, .01f))
	{
		if (ShaderManager::Get().depthPass == false)
		{
			ShaderManager::Get().shader->SetVector3(diffuse, diffuseString);
		}
	}
	if (ImGui::DragFloat3("specular", &specular.x, .01f))
	{
		if (ShaderManager::Get().depthPass == false)
		{
			ShaderManager::Get().shader->SetVector3(specular, specularString);
		}
	}
}
