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
	ShaderManager::Get().shader->SetVector3(ambient, ambientString);
	ShaderManager::Get().shader->SetVector3(diffuse, diffuseString);
	ShaderManager::Get().shader->SetVector3(specular, specularString);

	ShaderManager::Get().shader->SetFloat(linear, linearString);
	ShaderManager::Get().shader->SetFloat(quadratic, quadraticString);
}

void PointLight::SetToZero()
{
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), ambientString);
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), diffuseString);
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), specularString);

	ShaderManager::Get().shader->SetFloat(0.0f, linearString);
	ShaderManager::Get().shader->SetFloat(0.0f, quadraticString);
}

void PointLight::SetPosition(glm::vec3 pos)
{
	position = pos;
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
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->SetFloat(far_plane, "far_plane");
	}
	else
	{
		for (unsigned int i = 0; i < 6; i++)
		{
			ShaderManager::Get().depthCubeMapShader->SetMatrix4(shadowTransforms[i], "shadowMatrices[" + std::to_string(i) + "]"); //this is not going to work without the geometry shader initialized.
		}
		ShaderManager::Get().depthCubeMapShader->SetFloat(far_plane, "far_plane");
		//ShaderManager::Get().depthCubeMapShader->SetVector3(position, positionString); //not needed because we already do this above.
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
		SetToZero();
	}

	if (ImGui::DragFloat3("ambient", &ambient.x, .01f))
	{
		ShaderManager::Get().shader->SetVector3(ambient, ambientString);
	}
	if (ImGui::DragFloat3("diffuse", &diffuse.x, .01f))
	{
		ShaderManager::Get().shader->SetVector3(diffuse, diffuseString);
	}
	if (ImGui::DragFloat3("specular", &specular.x, .01f))
	{
		ShaderManager::Get().shader->SetVector3(specular, specularString);
	}

	if (ImGui::DragFloat("linear", &linear, .01f))
	{
		ShaderManager::Get().shader->SetFloat(linear, linearString);
	}
	if (ImGui::DragFloat("quadratic", &quadratic, .01f))
	{
		ShaderManager::Get().shader->SetFloat(quadratic, quadraticString);
	}
}

void PointLight::Serialization(std::fstream& file)
{
	int ambientSize = ambient.length();
	int diffuseSize = diffuse.length();
	int specularSize = specular.length();
	int linearSize = sizeof(float);
	int quadraticSize = sizeof(float);

	file.write(reinterpret_cast<char*>(&ambientSize), sizeof(ambientSize));
	file.write(reinterpret_cast<char*>(&diffuseSize), sizeof(diffuseSize));
	file.write(reinterpret_cast<char*>(&specularSize), sizeof(specularSize));
	file.write(reinterpret_cast<char*>(&linearSize), sizeof(linearSize));
	file.write(reinterpret_cast<char*>(&quadraticSize), sizeof(quadraticSize));

	file.write(reinterpret_cast<char*>(&ambient[0]), sizeof(glm::vec3) * ambientSize);
	file.write(reinterpret_cast<char*>(&diffuse[0]), sizeof(glm::vec3) * diffuseSize);
	file.write(reinterpret_cast<char*>(&specular[0]), sizeof(glm::vec3) * specularSize);
	file.write(reinterpret_cast<char*>(&linear), linearSize);
	file.write(reinterpret_cast<char*>(&quadratic), quadraticSize);
}

void PointLight::Deserialization(std::fstream& file)
{
	int ambientSize;
	int diffuseSize;
	int specularSize;
	int linearSize;
	int quadraticSize;

	file.read(reinterpret_cast<char*>(&ambientSize), sizeof(ambientSize));
	file.read(reinterpret_cast<char*>(&diffuseSize), sizeof(diffuseSize));
	file.read(reinterpret_cast<char*>(&specularSize), sizeof(specularSize));
	file.read(reinterpret_cast<char*>(&linearSize), sizeof(linearSize));
	file.read(reinterpret_cast<char*>(&quadraticSize), sizeof(quadraticSize));

	file.read(reinterpret_cast<char*>(&ambient[0]), sizeof(glm::vec3) * ambientSize);
	file.read(reinterpret_cast<char*>(&diffuse[0]), sizeof(glm::vec3) * diffuseSize);
	file.read(reinterpret_cast<char*>(&specular[0]), sizeof(glm::vec3) * specularSize);
	file.read(reinterpret_cast<char*>(&linear), linearSize);
	file.read(reinterpret_cast<char*>(&quadratic), quadraticSize);
}
