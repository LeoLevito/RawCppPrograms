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
		ambient = glm::vec3(0, 0, 0);
		diffuse = glm::vec3(1, 1, 1);
		specular = glm::vec3(1, 1, 1);

		shaderRef->SetVector3(ambient, "light.ambient");
		shaderRef->SetVector3(diffuse, "light.diffuse");
		shaderRef->SetVector3(specular, "light.specular");

		isShaderRefSet = true;
	}

	for (size_t i = 0; i < owner->components.size(); i++)
	{
		if (dynamic_cast<TransformComponent*>(owner->components[i])) //checking if owner has a component of type TransformComponent. Is of-type correct word-use in this case?
		{
			//multiple casts, in UPDATE()? not sure how efficient this is. EDIT 5 dec 2024, apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times.
			position = dynamic_cast<TransformComponent*>(owner->components[i])->position; //need to get a reference to the transform component if there is one.
			shader->SetVector3(position, "light.position"); //Light position will be kept when this component is deleted, because it was last updated here.
		}
	}
}

void LightComponent::DrawComponentSpecificImGuiHierarchyAdjustables()
{
	Component::DrawComponentSpecificImGuiHierarchyAdjustables();

	if (ImGui::DragFloat3("ambient", &ambient.x, .01f))
	{
		shaderRef->SetVector3(ambient, "light.ambient");
	}
	if (ImGui::DragFloat3("diffuse", &diffuse.x, .01f))
	{
		shaderRef->SetVector3(diffuse, "light.diffuse");
	}
	if (ImGui::DragFloat3("specular", &specular.x, .01f))
	{
		shaderRef->SetVector3(specular, "light.specular");
	}
}
