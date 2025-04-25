#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	IDstring.reserve(50);
}

DirectionalLight::~DirectionalLight()
{
	position = glm::vec3(0, 0, 0);
	direction = glm::vec3(0, 0, 0);
	ShaderManager::Get().shader->Use();
	ShaderManager::Get().shader->SetVector3(position, positionString);
	ShaderManager::Get().shader->SetVector3(direction, directionString);
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

	ShaderManager::Get().shader->Use();
	ShaderManager::Get().shader->SetVector3(ambient, ambientString);
	ShaderManager::Get().shader->SetVector3(diffuse, diffuseString);
	ShaderManager::Get().shader->SetVector3(specular, specularString);
}

void DirectionalLight::SetToCurrent()
{
	ShaderManager::Get().shader->Use();
	ShaderManager::Get().shader->SetVector3(ambient, ambientString);
	ShaderManager::Get().shader->SetVector3(diffuse, diffuseString);
	ShaderManager::Get().shader->SetVector3(specular, specularString);
}

void DirectionalLight::SetToZero(bool alsoSetVariables)
{
	if (alsoSetVariables) //if setting to zero was done by pressing the 'Set to zero' imgui::button, then we deliberately set variables to zero. Otherwise if this light is being deleted, then we don't do this.
	{
		ambient = glm::vec3(0, 0, 0);
		diffuse = glm::vec3(0, 0, 0);
		specular = glm::vec3(0, 0, 0);
	}

	ShaderManager::Get().shader->Use();
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), ambientString);
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), diffuseString);
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), specularString);
}

void DirectionalLight::SetPosition(glm::vec3 pos)
{
	position = pos;

	ShaderManager::Get().shader->Use();
	ShaderManager::Get().shader->SetVector3(position, positionString);

}

void DirectionalLight::SetDirection(glm::vec3 dir)
{
	direction = dir;

	ShaderManager::Get().shader->Use();
	ShaderManager::Get().shader->SetVector3(direction, directionString);

}

void DirectionalLight::SetLightSpaceMatrix()
{
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

	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->Use();
		ShaderManager::Get().shader->SetMatrix4(lightSpaceMatrix, "lightSpaceMatrix");
	}
	else
	{
		ShaderManager::Get().depthShader->Use();
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
		SetToZero(true);
	}

	if (ImGui::DragFloat3("ambient", &ambient.x, .01f))
	{

		ShaderManager::Get().shader->Use();
		ShaderManager::Get().shader->SetVector3(ambient, ambientString);

	}
	if (ImGui::DragFloat3("diffuse", &diffuse.x, .01f))
	{

		ShaderManager::Get().shader->Use();
		ShaderManager::Get().shader->SetVector3(diffuse, diffuseString);

	}
	if (ImGui::DragFloat3("specular", &specular.x, .01f))
	{

		ShaderManager::Get().shader->Use();
		ShaderManager::Get().shader->SetVector3(specular, specularString);

	}
}

void DirectionalLight::Serialization(std::fstream& file)
{
	file.write(reinterpret_cast<char*>(&ambient[0]), sizeof(glm::vec3));
	file.write(reinterpret_cast<char*>(&diffuse[0]), sizeof(glm::vec3));
	file.write(reinterpret_cast<char*>(&specular[0]), sizeof(glm::vec3));
}

void DirectionalLight::Deserialization(std::fstream& file)
{
	file.read(reinterpret_cast<char*>(&ambient[0]), sizeof(glm::vec3));
	file.read(reinterpret_cast<char*>(&diffuse[0]), sizeof(glm::vec3));
	file.read(reinterpret_cast<char*>(&specular[0]), sizeof(glm::vec3));


	ShaderManager::Get().shader->Use();
	ShaderManager::Get().shader->SetVector3(ambient, ambientString);
	ShaderManager::Get().shader->SetVector3(diffuse, diffuseString);
	ShaderManager::Get().shader->SetVector3(specular, specularString);

}
