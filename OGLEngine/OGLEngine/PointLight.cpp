#include "PointLight.h"

PointLight::PointLight()
{
}

void PointLight::UpdateIDBasedStrings()
{
	IDstring = "pointLights[";
	IDstring += std::to_string(ID);
	IDstring += "].";

	positionString = IDstring;
	positionString += "position";

	ambientString = IDstring;
	ambientString += "ambient";

	diffuseString = IDstring;
	diffuseString += "diffuse";

	specularString = IDstring;
	specularString += "specular";

	linearString = IDstring;
	linearString += "linear";

	quadraticString = IDstring;
	quadraticString += "quadratic";
}

void PointLight::SetToDefault()
{
	ShaderManager::Get().shader->Use();
	ambient = glm::vec3(0, 0, 0);
	diffuse = glm::vec3(1, 1, 1);
	specular = glm::vec3(1, 1, 1);
	ShaderManager::Get().shader->SetVector3(ambient, ambientString);
	ShaderManager::Get().shader->SetVector3(diffuse, diffuseString);
	ShaderManager::Get().shader->SetVector3(specular, specularString);

	linear = 0.09f;
	quadratic = 0.032f;
	ShaderManager::Get().shader->SetFloat(linear, linearString);
	ShaderManager::Get().shader->SetFloat(quadratic, quadraticString);
}

void PointLight::SetToCurrent()
{
	ShaderManager::Get().shader->Use();
	ShaderManager::Get().shader->SetVector3(ambient, ambientString);
	ShaderManager::Get().shader->SetVector3(diffuse, diffuseString);
	ShaderManager::Get().shader->SetVector3(specular, specularString);

	ShaderManager::Get().shader->SetFloat(linear, linearString);
	ShaderManager::Get().shader->SetFloat(quadratic, quadraticString);
}

void PointLight::SetToZero(bool alsoSetVariables)
{
	if (alsoSetVariables) //if setting to zero was done by pressing the 'Set to zero' imgui::button, then we deliberately set variables to zero. Otherwise if this light is being deleted, then we don't do this.
	{
		ambient = glm::vec3(0, 0, 0);
		diffuse = glm::vec3(0, 0, 0);
		specular = glm::vec3(0, 0, 0);

		linear = 0.0f;
		quadratic = 0.0f;
	}

	ShaderManager::Get().shader->Use();

	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), ambientString);
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), diffuseString);
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), specularString);

	ShaderManager::Get().shader->SetFloat(0.0f, linearString);
	ShaderManager::Get().shader->SetFloat(0.0f, quadraticString);
}

void PointLight::SetPosition(glm::vec3 pos)
{
	position = pos;
	ShaderManager::Get().shader->Use();
	ShaderManager::Get().shader->SetVector3(position, positionString);
}

void PointLight::SetLightSpaceMatrix()
{
	glm::mat4 shadowProj;
	glm::vec3 lightPos = position;
	float aspect = (float)ShaderManager::Get().shadowCubeMap->SHADOW_WIDTH / (float)ShaderManager::Get().shadowCubeMap->SHADOW_HEIGHT;
	float near_plane = 0.001f;
	float far_plane = 100.f;
	shadowProj = glm::perspective(glm::radians(90.f), (float)ShaderManager::Get().shadowCubeMap->SHADOW_WIDTH / (float)ShaderManager::Get().shadowCubeMap->SHADOW_HEIGHT, near_plane, far_plane);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->Use();
		ShaderManager::Get().shader->SetFloat(far_plane, "far_plane");
	}
	else
	{
		ShaderManager::Get().depthCubeMapShader->Use();
		for (unsigned int i = 0; i < 6; i++)
		{
			ShaderManager::Get().depthCubeMapShader->SetMatrix4(shadowTransforms[i], "shadowMatrices[" + std::to_string(i) + "]"); //this is not going to work without the geometry shader initialized.
		}
		ShaderManager::Get().depthCubeMapShader->SetFloat(far_plane, "far_plane");
		ShaderManager::Get().depthCubeMapShader->SetVector3(position, "lightPos"); //not needed because we already do this above.
	}
}

void PointLight::DrawImgui()
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

	if (ImGui::DragFloat("linear", &linear, .01f))
	{
		ShaderManager::Get().shader->Use();
		ShaderManager::Get().shader->SetFloat(linear, linearString);
	}
	if (ImGui::DragFloat("quadratic", &quadratic, .01f))
	{
		ShaderManager::Get().shader->Use();
		ShaderManager::Get().shader->SetFloat(quadratic, quadraticString);
	}
}

void PointLight::Serialization(std::fstream& file)
{
	file.write(reinterpret_cast<char*>(&ambient[0]), sizeof(glm::vec3));
	file.write(reinterpret_cast<char*>(&diffuse[0]), sizeof(glm::vec3));
	file.write(reinterpret_cast<char*>(&specular[0]), sizeof(glm::vec3));
	file.write(reinterpret_cast<char*>(&linear), sizeof(float));
	file.write(reinterpret_cast<char*>(&quadratic), sizeof(float));
}

void PointLight::Deserialization(std::fstream& file)
{
	file.read(reinterpret_cast<char*>(&ambient[0]), sizeof(glm::vec3));
	file.read(reinterpret_cast<char*>(&diffuse[0]), sizeof(glm::vec3));
	file.read(reinterpret_cast<char*>(&specular[0]), sizeof(glm::vec3));
	file.read(reinterpret_cast<char*>(&linear), sizeof(float));
	file.read(reinterpret_cast<char*>(&quadratic), sizeof(float));



	ShaderManager::Get().shader->Use();
	ShaderManager::Get().shader->SetVector3(ambient, ambientString);
	ShaderManager::Get().shader->SetVector3(diffuse, diffuseString);
	ShaderManager::Get().shader->SetVector3(specular, specularString);

	ShaderManager::Get().shader->SetFloat(linear, linearString);
	ShaderManager::Get().shader->SetFloat(quadratic, quadraticString);

}
