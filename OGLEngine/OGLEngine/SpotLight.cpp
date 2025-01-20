#include "SpotLight.h"

SpotLight::SpotLight()
{
}

void SpotLight::UpdateIDBasedStrings()
{
	IDstring = "spotLights[";
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

	linearString = IDstring;
	linearString += "linear";

	quadraticString = IDstring;
	quadraticString += "quadratic";

	cutoffString = IDstring;
	cutoffString += "cutoff";

	outerCutoffString = IDstring;
	outerCutoffString += "outerCutoff";
}

void SpotLight::SetToDefault()
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


	cutoff = 12.5f;
	outerCutoff = 17.5f;
	float myCutoff = glm::cos(glm::radians(cutoff));
	float myOuterCutoff = glm::cos(glm::radians(outerCutoff));
	ShaderManager::Get().shader->SetFloat(myCutoff, cutoffString);
	ShaderManager::Get().shader->SetFloat(myOuterCutoff, outerCutoffString);
}

void SpotLight::SetToCurrent()
{
	ShaderManager::Get().shader->SetVector3(ambient, ambientString);
	ShaderManager::Get().shader->SetVector3(diffuse, diffuseString);
	ShaderManager::Get().shader->SetVector3(specular, specularString);

	ShaderManager::Get().shader->SetFloat(linear, linearString);
	ShaderManager::Get().shader->SetFloat(quadratic, quadraticString);

	float myCutoff = glm::cos(glm::radians(cutoff));
	float myOuterCutoff = glm::cos(glm::radians(outerCutoff));
	ShaderManager::Get().shader->SetFloat(myCutoff, cutoffString);
	ShaderManager::Get().shader->SetFloat(myOuterCutoff, outerCutoffString);
}

void SpotLight::SetToZero()
{
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), ambientString);
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), diffuseString);
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), specularString);

	ShaderManager::Get().shader->SetFloat(0.0f, linearString);
	ShaderManager::Get().shader->SetFloat(0.0f, quadraticString);

	float myCutoff = glm::cos(glm::radians(0.0f));
	float myOuterCutoff = glm::cos(glm::radians(0.0f));
	ShaderManager::Get().shader->SetFloat(myCutoff, cutoffString);
	ShaderManager::Get().shader->SetFloat(myOuterCutoff, outerCutoffString);
}

void SpotLight::SetPosition(glm::vec3 pos)
{
	position = pos;
	ShaderManager::Get().shader->SetVector3(position, positionString);
}

void SpotLight::SetDirection(glm::vec3 dir)
{
	direction = dir;
	ShaderManager::Get().shader->SetVector3(direction, directionString);
}

void SpotLight::DrawImgui()
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

	if (ImGui::DragFloat("cutoff", &cutoff, .01f))
	{
		float myCutoff = glm::cos(glm::radians(cutoff)); //calculate cosine value before handing it to the fragment shader because it's an expensive operation and we don't wanna do that inside the shader.
		ShaderManager::Get().shader->SetFloat(myCutoff, cutoffString);
	}
	if (ImGui::DragFloat("outerCutoff", &outerCutoff, .01f))
	{
		float myOuterCutoff = glm::cos(glm::radians(outerCutoff)); //calculate cosine value before handing it to the fragment shader because it's an expensive operation and we don't wanna do that inside the shader.
		ShaderManager::Get().shader->SetFloat(myOuterCutoff, outerCutoffString);
	}
}
