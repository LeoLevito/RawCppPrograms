#include "LightComponent.h"
#include <GameObject.h>
#include <TransformComponent.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "LightManager.h"
#include "ShaderManager.h"

LightComponent::LightComponent()
{
	name = "Light component";
	myLight = LightManager::Get().AddNewLight(DirectionalLightType);
	myLight->SetToDefault(); //what's the next step? Setting direction, ambient, diffuse, specular? Maybe that ImGui stuff should be inside each Light class?



	//will replace this code
	//SetDirectionalLightToDefault();

	SetPointLightToDefault();
	SetPointLightToZero();

	SetSpotLightToDefault();
	SetSpotLightToZero();
}

LightComponent::~LightComponent()
{
	std::cout << "-->Deleting Light component." << std::endl;
}

void LightComponent::Update()
{
	for (size_t i = 0; i < owner->components.size(); i++)
	{
		if (dynamic_cast<TransformComponent*>(owner->components[i])) //checking if owner has a component of type TransformComponent. Is of-type correct word-use in this case?
		{
			//multiple casts, in UPDATE()? not sure how efficient this is. EDIT 5 dec 2024, apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times.
			position = dynamic_cast<TransformComponent*>(owner->components[i])->position; //need to get a reference to the transform component if there is one.
			rotation = dynamic_cast<TransformComponent*>(owner->components[i])->rotation;
			//scale = dynamic_cast<TransformComponent*>(owner->components[i])->scale;

			ShaderManager::Get().shader->SetVector3(rotation, "directionalLight.direction");
			ShaderManager::Get().shader->SetVector3(rotation, "spotLight.direction");

			int myInt = owner->ID;
			std::string str0 = "pointLights[";
			std::string str1 = std::to_string(myInt);
			std::string str2 = "].position";
			str0.append(str1);
			str0.append(str2);
			
			ShaderManager::Get().shader->SetVector3(position, str0); //I need some kind of ID system, maybe calling to a LightManager.
			ShaderManager::Get().shader->SetVector3(position, "spotLight.position");
		}
	}
}

void LightComponent::DrawComponentSpecificImGuiHierarchyAdjustables()
{
	Component::DrawComponentSpecificImGuiHierarchyAdjustables();

	const char* typeNames[] = { "Directional light", "Point light", "Spot light" };

	ImGui::Text("Current type:");
	ImGui::Text(typeNames[selectedType]);

	if (ImGui::Button("Select type"))
	{
		ImGui::OpenPopup("Light Type Popup");
	}

	if (ImGui::BeginPopup("Light Type Popup"))
	{
		ImGui::SeparatorText("TYPES:");
		for (int i = 0; i < IM_ARRAYSIZE(typeNames); i++)
		{
			if (ImGui::Selectable(typeNames[i]))
			{
				selectedType = i;
				switch (selectedType)
				{
				case 0:
					SetPointLightToZero(); //should save values before setting to zero. Maybe I don't even need to do that and just set them to zero directly.
					SetSpotLightToZero();

					SetDirectionalLightToCurrent(); //instead of setting it to default each time, set it to last saved values if user wants to change back the light to this type.
					break;
				case 1:
					SetDirectionalLightToZero();
					SetSpotLightToZero();

					SetPointLightToCurrent();
					break;
				case 2:
					SetDirectionalLightToZero();
					SetPointLightToZero();

					SetSpotLightToCurrent();
					break;
				default:
					break;
				}
			}
		}
		ImGui::EndPopup();
	}

	//directional light parameters
	if (selectedType == 0) 
	{
		//if (ImGui::Button("reset to defaults"))
		//{
		//	SetDirectionalLightToDefault();
		//}
		//if (ImGui::Button("reset to zero"))
		//{
		//	SetDirectionalLightToZero();
		//}

		//if (ImGui::DragFloat3("ambient", &directionalAmbient.x, .01f))
		//{
		//	ShaderManager::Get().shader->SetVector3(directionalAmbient, "directionalLight.ambient");
		//}
		//if (ImGui::DragFloat3("diffuse", &directionalDiffuse.x, .01f))
		//{
		//	ShaderManager::Get().shader->SetVector3(directionalDiffuse, "directionalLight.diffuse");
		//}
		//if (ImGui::DragFloat3("specular", &directionalSpecular.x, .01f))
		//{
		//	ShaderManager::Get().shader->SetVector3(directionalSpecular, "directionalLight.specular");
		//}
	}

	//point light parameters
	if (selectedType == 1) 
	{
		if (ImGui::Button("reset to defaults"))
		{
			SetPointLightToDefault();
		}
		if (ImGui::Button("reset to zero"))
		{
			SetPointLightToZero();
		}

		if (ImGui::DragFloat3("ambient", &pointAmbient.x, .01f))
		{
			int myInt = owner->ID;
			std::string str0 = "pointLights[";
			std::string str1 = std::to_string(myInt);
			std::string str2 = "].";
			str0.append(str1);
			str0.append(str2);
			ShaderManager::Get().shader->SetVector3(pointAmbient, str0.append("ambient"));
		}
		if (ImGui::DragFloat3("diffuse", &pointDiffuse.x, .01f))
		{
			int myInt = owner->ID;
			std::string str0 = "pointLights[";
			std::string str1 = std::to_string(myInt);
			std::string str2 = "].";
			str0.append(str1);
			str0.append(str2);
			ShaderManager::Get().shader->SetVector3(pointDiffuse, str0.append("diffuse"));
		}
		if (ImGui::DragFloat3("specular", &pointSpecular.x, .01f))
		{
			int myInt = owner->ID;
			std::string str0 = "pointLights[";
			std::string str1 = std::to_string(myInt);
			std::string str2 = "].";
			str0.append(str1);
			str0.append(str2);
			ShaderManager::Get().shader->SetVector3(pointSpecular, str0.append("specular"));
		}

		if (ImGui::DragFloat("linear", &pointLinear, .01f))
		{
			int myInt = owner->ID;
			std::string str0 = "pointLights[";
			std::string str1 = std::to_string(myInt);
			std::string str2 = "].";
			str0.append(str1);
			str0.append(str2);
			ShaderManager::Get().shader->SetFloat(pointLinear, str0.append("linear"));
		}
		if (ImGui::DragFloat("quadratic", &pointQuadratic, .01f))
		{
			int myInt = owner->ID;
			std::string str0 = "pointLights[";
			std::string str1 = std::to_string(myInt);
			std::string str2 = "].";
			str0.append(str1);
			str0.append(str2);
			ShaderManager::Get().shader->SetFloat(pointQuadratic, str0.append("quadratic"));
		}
	}

	//spot light parameters
	if (selectedType == 2) 
	{
		if (ImGui::Button("reset to defaults"))
		{
			SetSpotLightToDefault();
		}
		if (ImGui::Button("reset to zero"))
		{
			SetSpotLightToZero();
		}

		if (ImGui::DragFloat3("ambient", &spotAmbient.x, .01f))
		{
			ShaderManager::Get().shader->SetVector3(spotAmbient, "spotLight.ambient");
		}
		if (ImGui::DragFloat3("diffuse", &spotDiffuse.x, .01f))
		{
			ShaderManager::Get().shader->SetVector3(spotDiffuse, "spotLight.diffuse");
		}
		if (ImGui::DragFloat3("specular", &spotSpecular.x, .01f))
		{
			ShaderManager::Get().shader->SetVector3(spotSpecular, "spotLight.specular");
		}

		if (ImGui::DragFloat("linear", &spotLinear, .01f))
		{
			ShaderManager::Get().shader->SetFloat(spotLinear, "spotLight.linear");
		}
		if (ImGui::DragFloat("quadratic", &spotQuadratic, .01f))
		{
			ShaderManager::Get().shader->SetFloat(spotQuadratic, "spotLight.quadratic");
		}

		if (ImGui::DragFloat("cutoff", &spotCutoff, .01f))
		{
			float myCutoff = glm::cos(glm::radians(spotCutoff)); //calculate cosine value before handing it to the fragment shader because it's an expensive operation and we don't wanna do that inside the shader.
			ShaderManager::Get().shader->SetFloat(myCutoff, "spotLight.cutoff");
		}
		if (ImGui::DragFloat("outerCutoff", &spotOuterCutoff, .01f))
		{
			float myOuterCutoff = glm::cos(glm::radians(spotOuterCutoff)); //calculate cosine value before handing it to the fragment shader because it's an expensive operation and we don't wanna do that inside the shader.
			ShaderManager::Get().shader->SetFloat(myOuterCutoff, "spotLight.outerCutoff");
		}
	}
}

void LightComponent::SetDirectionalLightToDefault()
{
	//directionalAmbient = glm::vec3(0, 0, 0);
	//directionalDiffuse = glm::vec3(1, 1, 1);
	//directionalSpecular = glm::vec3(1, 1, 1);
	//ShaderManager::Get().shader->SetVector3(directionalAmbient, "directionalLight.ambient");
	//ShaderManager::Get().shader->SetVector3(directionalDiffuse, "directionalLight.diffuse");
	//ShaderManager::Get().shader->SetVector3(directionalSpecular, "directionalLight.specular");
}

void LightComponent::SetDirectionalLightToCurrent()
{
	//ShaderManager::Get().shader->SetVector3(directionalAmbient, "directionalLight.ambient");
	//ShaderManager::Get().shader->SetVector3(directionalDiffuse, "directionalLight.diffuse");
	//ShaderManager::Get().shader->SetVector3(directionalSpecular, "directionalLight.specular");
}

void LightComponent::SetDirectionalLightToZero()
{
	//ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), "directionalLight.ambient");
	//ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), "directionalLight.diffuse");
	//ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), "directionalLight.specular");
}

void LightComponent::SetPointLightToDefault()
{
	int myInt = owner->ID;
	std::string str0 = "pointLights[";
	std::string str1 = std::to_string(myInt);
	std::string str2 = "].";
	str0.append(str1);
	str0.append(str2);

	std::string ambientstring = str0;
	ambientstring.append("ambient");

	std::string diffusestring = str0;
	diffusestring.append("diffuse");

	std::string specularstring = str0;
	specularstring.append("specular");

	std::string linearstring = str0;
	linearstring.append("linear");

	std::string quadraticstring = str0;
	quadraticstring.append("quadratic");

	pointAmbient = glm::vec3(0, 0, 0);
	pointDiffuse = glm::vec3(1, 1, 1);
	pointSpecular = glm::vec3(1, 1, 1);
	ShaderManager::Get().shader->SetVector3(pointAmbient, ambientstring);
	ShaderManager::Get().shader->SetVector3(pointDiffuse, diffusestring);
	ShaderManager::Get().shader->SetVector3(pointSpecular, specularstring);

	pointLinear = 0.09f;
	pointQuadratic = 0.032f;
	ShaderManager::Get().shader->SetFloat(pointLinear, linearstring);
	ShaderManager::Get().shader->SetFloat(pointQuadratic, quadraticstring);
}

void LightComponent::SetPointLightToCurrent()
{
	int myInt = owner->ID;
	std::string str0 = "pointLights[";
	std::string str1 = std::to_string(myInt);
	std::string str2 = "].";
	str0.append(str1);
	str0.append(str2);

	std::string ambientstring = str0;
	ambientstring.append("ambient");

	std::string diffusestring = str0;
	diffusestring.append("diffuse");

	std::string specularstring = str0;
	specularstring.append("specular");

	std::string linearstring = str0;
	linearstring.append("linear");

	std::string quadraticstring = str0;
	quadraticstring.append("quadratic");

	ShaderManager::Get().shader->SetVector3(pointAmbient, ambientstring);
	ShaderManager::Get().shader->SetVector3(pointDiffuse, diffusestring);
	ShaderManager::Get().shader->SetVector3(pointSpecular, specularstring);

	ShaderManager::Get().shader->SetFloat(pointLinear, linearstring);
	ShaderManager::Get().shader->SetFloat(pointQuadratic, quadraticstring);
}

void LightComponent::SetPointLightToZero()
{
	int myInt = owner->ID;
	std::string str0 = "pointLights[";
	std::string str1 = std::to_string(myInt);
	std::string str2 = "].";
	str0.append(str1);
	str0.append(str2);

	std::string ambientstring = str0;
	ambientstring.append("ambient");

	std::string diffusestring = str0;
	diffusestring.append("diffuse");

	std::string specularstring = str0;
	specularstring.append("specular");

	std::string linearstring = str0;
	linearstring.append("linear");

	std::string quadraticstring = str0;
	quadraticstring.append("quadratic");

	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), ambientstring);
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), diffusestring);
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), specularstring);

	ShaderManager::Get().shader->SetFloat(0.0f, linearstring);
	ShaderManager::Get().shader->SetFloat(0.0f, quadraticstring);
}

void LightComponent::SetSpotLightToDefault()
{
	spotAmbient = glm::vec3(0, 0, 0);
	spotDiffuse = glm::vec3(1, 1, 1);
	spotSpecular = glm::vec3(1, 1, 1);
	ShaderManager::Get().shader->SetVector3(spotAmbient, "spotLight.ambient");
	ShaderManager::Get().shader->SetVector3(spotDiffuse, "spotLight.diffuse");
	ShaderManager::Get().shader->SetVector3(spotSpecular, "spotLight.specular");

	spotLinear = 0.09f;
	spotQuadratic = 0.032f;
	ShaderManager::Get().shader->SetFloat(spotLinear, "spotLight.linear");
	ShaderManager::Get().shader->SetFloat(spotQuadratic, "spotLight.quadratic");

	spotCutoff = 12.5f;
	spotOuterCutoff = 17.5f;
	float myCutoff = glm::cos(glm::radians(spotCutoff));
	float myOuterCutoff = glm::cos(glm::radians(spotOuterCutoff));
	ShaderManager::Get().shader->SetFloat(myCutoff, "spotLight.cutoff");
	ShaderManager::Get().shader->SetFloat(myOuterCutoff, "spotLight.outerCutoff");
}

void LightComponent::SetSpotLightToCurrent()
{
	ShaderManager::Get().shader->SetVector3(spotAmbient, "spotLight.ambient");
	ShaderManager::Get().shader->SetVector3(spotDiffuse, "spotLight.diffuse");
	ShaderManager::Get().shader->SetVector3(spotSpecular, "spotLight.specular");

	ShaderManager::Get().shader->SetFloat(spotLinear, "spotLight.linear");
	ShaderManager::Get().shader->SetFloat(spotQuadratic, "spotLight.quadratic");

	float myCutoff = glm::cos(glm::radians(spotCutoff));
	float myOuterCutoff = glm::cos(glm::radians(spotOuterCutoff));
	ShaderManager::Get().shader->SetFloat(myCutoff, "spotLight.cutoff");
	ShaderManager::Get().shader->SetFloat(myOuterCutoff, "spotLight.outerCutoff");
}

void LightComponent::SetSpotLightToZero()
{
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), "spotLight.ambient");
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), "spotLight.diffuse");
	ShaderManager::Get().shader->SetVector3(glm::vec3(0, 0, 0), "spotLight.specular");

	ShaderManager::Get().shader->SetFloat(0.0f, "spotLight.linear");
	ShaderManager::Get().shader->SetFloat(0.0f, "spotLight.quadratic");

	float myCutoff = glm::cos(glm::radians(0.0f));
	float myOuterCutoff = glm::cos(glm::radians(0.0f));
	ShaderManager::Get().shader->SetFloat(myCutoff, "spotLight.cutoff");
	ShaderManager::Get().shader->SetFloat(myOuterCutoff, "spotLight.outerCutoff");
}
