#include "LightComponent.h"
#include <GameObject.h>
#include <TransformComponent.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

LightComponent::LightComponent()
{
	name = "Light component";
}

LightComponent::~LightComponent()
{
	std::cout << "-->Deleting Light component." << std::endl;
}

void LightComponent::Update(Shader* shader)
{
	if (!isShaderRefSet) //I need to get rid of this shader pointer in the Update function. Maybe a ShaderManager would work.
	{
		Shader& realShaderRef = *shader;
		shaderRef = &realShaderRef;

		//setting default values for light behavior.
		directionalAmbient = glm::vec3(0, 0, 0);
		directionalDiffuse = glm::vec3(1, 1, 1);
		directionalSpecular = glm::vec3(1, 1, 1);

		shaderRef->SetVector3(directionalAmbient, "directionalLight.ambient");
		//shaderRef->SetVector3(ambient, "pointLight.ambient");
		//shaderRef->SetVector3(ambient, "spotLight.ambient");

		shaderRef->SetVector3(directionalDiffuse, "directionalLight.diffuse");
		//shaderRef->SetVector3(diffuse, "pointLight.diffuse");
		//shaderRef->SetVector3(diffuse, "spotLight.diffuse");

		shaderRef->SetVector3(directionalSpecular, "directionalLight.specular");
		//shaderRef->SetVector3(specular, "pointLight.specular");
		//shaderRef->SetVector3(specular, "spotLight.specular");

		isShaderRefSet = true;
	}

	for (size_t i = 0; i < owner->components.size(); i++)
	{
		if (dynamic_cast<TransformComponent*>(owner->components[i])) //checking if owner has a component of type TransformComponent. Is of-type correct word-use in this case?
		{
			//multiple casts, in UPDATE()? not sure how efficient this is. EDIT 5 dec 2024, apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times.
			position = dynamic_cast<TransformComponent*>(owner->components[i])->position; //need to get a reference to the transform component if there is one.
			rotation = dynamic_cast<TransformComponent*>(owner->components[i])->rotation;
			//scale = dynamic_cast<TransformComponent*>(owner->components[i])->scale;

			shader->SetVector3(rotation, "directionalLight.direction");
			shader->SetVector3(rotation, "spotLight.direction");

			std::string str0 = "pointLights[";
			int myInt = 0;
			std::string str1 = std::to_string(myInt);
			std::string str2 = "].position";
			str0.append(str1);
			str0.append(str2);
			
			shader->SetVector3(position, str0); //I need some kind of ID system, maybe calling to a LightManager.
			shader->SetVector3(position, "spotLight.position");
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

	if (selectedType == 0) //directional light
	{
		if (ImGui::Button("reset to defaults"))
		{
			SetDirectionalLightToDefault();
		}
		if (ImGui::Button("reset to zero"))
		{
			SetDirectionalLightToDefault();
		}

		if (ImGui::DragFloat3("ambient", &directionalAmbient.x, .01f))
		{
			shaderRef->SetVector3(directionalAmbient, "directionalLight.ambient");
		}
		if (ImGui::DragFloat3("diffuse", &directionalDiffuse.x, .01f))
		{
			shaderRef->SetVector3(directionalDiffuse, "directionalLight.diffuse");
		}
		if (ImGui::DragFloat3("specular", &directionalSpecular.x, .01f))
		{
			shaderRef->SetVector3(directionalSpecular, "directionalLight.specular");
		}
	}

	if (selectedType == 1) //point light
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
			shaderRef->SetVector3(pointAmbient, "pointLights[0].ambient");
		}
		if (ImGui::DragFloat3("diffuse", &pointDiffuse.x, .01f))
		{
			shaderRef->SetVector3(pointDiffuse, "pointLights[0].diffuse");
		}
		if (ImGui::DragFloat3("specular", &pointSpecular.x, .01f))
		{
			shaderRef->SetVector3(pointSpecular, "pointLights[0].specular");
		}

		if (ImGui::DragFloat("linear", &pointLinear, .01f))
		{
			shaderRef->SetFloat(pointLinear, "pointLights[0].linear");
		}
		if (ImGui::DragFloat("quadratic", &pointQuadratic, .01f))
		{
			shaderRef->SetFloat(pointQuadratic, "pointLights[0].quadratic");
		}
	}

	if (selectedType == 2) //spot light
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
			shaderRef->SetVector3(spotAmbient, "spotLight.ambient");
		}
		if (ImGui::DragFloat3("diffuse", &spotDiffuse.x, .01f))
		{
			shaderRef->SetVector3(spotDiffuse, "spotLight.diffuse");
		}
		if (ImGui::DragFloat3("specular", &spotSpecular.x, .01f))
		{
			shaderRef->SetVector3(spotSpecular, "spotLight.specular");
		}

		if (ImGui::DragFloat("linear", &spotLinear, .01f))
		{
			shaderRef->SetFloat(spotLinear, "spotLight.linear");
		}
		if (ImGui::DragFloat("quadratic", &spotQuadratic, .01f))
		{
			shaderRef->SetFloat(spotQuadratic, "spotLight.quadratic");
		}

		if (ImGui::DragFloat("cutoff", &spotCutoff, .01f))
		{
			float myCutoff = glm::cos(glm::radians(spotCutoff)); //calculate cosine value before handing it to the fragment shader because it's an expensive operation and we don't wanna do that inside the shader.
			shaderRef->SetFloat(myCutoff, "spotLight.cutoff");
		}
		if (ImGui::DragFloat("outerCutoff", &spotOuterCutoff, .01f))
		{
			float myOuterCutoff = glm::cos(glm::radians(spotOuterCutoff)); //calculate cosine value before handing it to the fragment shader because it's an expensive operation and we don't wanna do that inside the shader.
			shaderRef->SetFloat(myOuterCutoff, "spotLight.outerCutoff");
		}
	}
}

void LightComponent::SetDirectionalLightToDefault()
{
	directionalAmbient = glm::vec3(0, 0, 0);
	directionalDiffuse = glm::vec3(1, 1, 1);
	directionalSpecular = glm::vec3(1, 1, 1);
	shaderRef->SetVector3(directionalAmbient, "directionalLight.ambient");
	shaderRef->SetVector3(directionalDiffuse, "directionalLight.diffuse");
	shaderRef->SetVector3(directionalSpecular, "directionalLight.specular");
}

void LightComponent::SetDirectionalLightToCurrent()
{
	shaderRef->SetVector3(directionalAmbient, "directionalLight.ambient");
	shaderRef->SetVector3(directionalDiffuse, "directionalLight.diffuse");
	shaderRef->SetVector3(directionalSpecular, "directionalLight.specular");
}

void LightComponent::SetDirectionalLightToZero()
{
	shaderRef->SetVector3(glm::vec3(0, 0, 0), "directionalLight.ambient");
	shaderRef->SetVector3(glm::vec3(0, 0, 0), "directionalLight.diffuse");
	shaderRef->SetVector3(glm::vec3(0, 0, 0), "directionalLight.specular");
}

void LightComponent::SetPointLightToDefault()
{
	pointAmbient = glm::vec3(0, 0, 0);
	pointDiffuse = glm::vec3(1, 1, 1);
	pointSpecular = glm::vec3(1, 1, 1);
	shaderRef->SetVector3(pointAmbient, "pointLights[0].ambient");
	shaderRef->SetVector3(pointDiffuse, "pointLights[0].diffuse");
	shaderRef->SetVector3(pointSpecular, "pointLights[0].specular");

	pointLinear = 0.09f;
	pointQuadratic = 0.032f;
	shaderRef->SetFloat(pointLinear, "pointLights[0].linear");
	shaderRef->SetFloat(pointQuadratic, "pointLights[0].quadratic");
}

void LightComponent::SetPointLightToCurrent()
{
	shaderRef->SetVector3(pointAmbient, "pointLights[0].ambient");
	shaderRef->SetVector3(pointDiffuse, "pointLights[0].diffuse");
	shaderRef->SetVector3(pointSpecular, "pointLights[0].specular");

	shaderRef->SetFloat(pointLinear, "pointLights[0].linear");
	shaderRef->SetFloat(pointQuadratic, "pointLights[0].quadratic");
}

void LightComponent::SetPointLightToZero()
{
	shaderRef->SetVector3(glm::vec3(0, 0, 0), "pointLights[0].ambient");
	shaderRef->SetVector3(glm::vec3(0, 0, 0), "pointLights[0].diffuse");
	shaderRef->SetVector3(glm::vec3(0, 0, 0), "pointLights[0].specular");

	shaderRef->SetFloat(0.0f, "pointLights[0].linear");
	shaderRef->SetFloat(0.0f, "pointLights[0].quadratic");
}

void LightComponent::SetSpotLightToDefault()
{
	spotAmbient = glm::vec3(0, 0, 0);
	spotDiffuse = glm::vec3(1, 1, 1);
	spotSpecular = glm::vec3(1, 1, 1);
	shaderRef->SetVector3(spotAmbient, "spotLight.ambient");
	shaderRef->SetVector3(spotDiffuse, "spotLight.diffuse");
	shaderRef->SetVector3(spotSpecular, "spotLight.specular");

	spotLinear = 0.09f;
	spotQuadratic = 0.032f;
	shaderRef->SetFloat(spotLinear, "spotLight.linear");
	shaderRef->SetFloat(spotQuadratic, "spotLight.quadratic");

	spotCutoff = 12.5f;
	spotOuterCutoff = 17.5f;
	float myCutoff = glm::cos(glm::radians(spotCutoff));
	float myOuterCutoff = glm::cos(glm::radians(spotOuterCutoff));
	shaderRef->SetFloat(myCutoff, "spotLight.cutoff");
	shaderRef->SetFloat(myOuterCutoff, "spotLight.outerCutoff");
}

void LightComponent::SetSpotLightToCurrent()
{
	shaderRef->SetVector3(spotAmbient, "spotLight.ambient");
	shaderRef->SetVector3(spotDiffuse, "spotLight.diffuse");
	shaderRef->SetVector3(spotSpecular, "spotLight.specular");

	shaderRef->SetFloat(spotLinear, "spotLight.linear");
	shaderRef->SetFloat(spotQuadratic, "spotLight.quadratic");

	float myCutoff = glm::cos(glm::radians(spotCutoff));
	float myOuterCutoff = glm::cos(glm::radians(spotOuterCutoff));
	shaderRef->SetFloat(myCutoff, "spotLight.cutoff");
	shaderRef->SetFloat(myOuterCutoff, "spotLight.outerCutoff");
}

void LightComponent::SetSpotLightToZero()
{
	shaderRef->SetVector3(glm::vec3(0, 0, 0), "spotLight.ambient");
	shaderRef->SetVector3(glm::vec3(0, 0, 0), "spotLight.diffuse");
	shaderRef->SetVector3(glm::vec3(0, 0, 0), "spotLight.specular");

	shaderRef->SetFloat(0.0f, "spotLight.linear");
	shaderRef->SetFloat(0.0f, "spotLight.quadratic");

	float myCutoff = glm::cos(glm::radians(0.0f));
	float myOuterCutoff = glm::cos(glm::radians(0.0f));
	shaderRef->SetFloat(myCutoff, "spotLight.cutoff");
	shaderRef->SetFloat(myOuterCutoff, "spotLight.outerCutoff");
}
