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

		shaderRef->SetVector3(ambient, "directionalLight.ambient");
		//shaderRef->SetVector3(ambient, "pointLight.ambient");
		//shaderRef->SetVector3(ambient, "spotLight.ambient");

		shaderRef->SetVector3(diffuse, "directionalLight.diffuse");
		//shaderRef->SetVector3(diffuse, "pointLight.diffuse");
		//shaderRef->SetVector3(diffuse, "spotLight.diffuse");

		shaderRef->SetVector3(specular, "directionalLight.specular");
		//shaderRef->SetVector3(specular, "pointLight.specular");
		//shaderRef->SetVector3(specular, "spotLight.specular");


		//constant = 1.0f;
		//linear = 0.09f;
		//quadratic = 0.032f;

		//shaderRef->SetFloat(constant, "pointLight.constant");
		//shaderRef->SetFloat(constant, "spotLight.constant");

		//shaderRef->SetFloat(linear, "pointLight.linear");
		//shaderRef->SetFloat(linear, "spotLight.linear");

		//shaderRef->SetFloat(quadratic, "pointLight.quadratic");
		//shaderRef->SetFloat(quadratic, "spotLight.quadratic");

		//cutoff = glm::cos(glm::radians(12.5f)); //calculate cosine value before handing it to the fragment shader because it's an expensive operation and we don't wanna do that inside the shader.
		//outerCutoff = glm::cos(glm::radians(17.5f)); //calculate cosine value before handing it to the fragment shader because it's an expensive operation and we don't wanna do that inside the shader.
		//
		//shaderRef->SetFloat(cutoff, "spotLight.cutoff");
		//shaderRef->SetFloat(cutoff, "spotLight.outerCutoff");

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

			shader->SetVector3(position, "pointLight.position");
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
			}
		}
		ImGui::EndPopup();
	}

	if (selectedType == 0) //directional light
	{
		if (ImGui::Button("reset to defaults"))
		{
			ambient = glm::vec3(0, 0, 0);
			diffuse = glm::vec3(1, 1, 1);
			specular = glm::vec3(1, 1, 1);
			shaderRef->SetVector3(ambient, "directionalLight.ambient");
			shaderRef->SetVector3(diffuse, "directionalLight.diffuse");
			shaderRef->SetVector3(specular, "directionalLight.specular");
		}
		if (ImGui::Button("reset to zero"))
		{
			ambient = glm::vec3(0, 0, 0);
			diffuse = glm::vec3(0, 0, 0);
			specular = glm::vec3(0, 0, 0);
			shaderRef->SetVector3(ambient, "directionalLight.ambient");
			shaderRef->SetVector3(diffuse, "directionalLight.diffuse");
			shaderRef->SetVector3(specular, "directionalLight.specular");
		}

		if (ImGui::DragFloat3("ambient", &ambient.x, .01f))
		{
			shaderRef->SetVector3(ambient, "directionalLight.ambient");
		}
		if (ImGui::DragFloat3("diffuse", &diffuse.x, .01f))
		{
			shaderRef->SetVector3(diffuse, "directionalLight.diffuse");
		}
		if (ImGui::DragFloat3("specular", &specular.x, .01f))
		{
			shaderRef->SetVector3(specular, "directionalLight.specular");
		}
	}

	if (selectedType == 1) //point light
	{
		if (ImGui::Button("reset to defaults"))
		{
			ambient = glm::vec3(0, 0, 0);
			diffuse = glm::vec3(1, 1, 1);
			specular = glm::vec3(1, 1, 1);
			shaderRef->SetVector3(ambient, "pointLight.ambient");
			shaderRef->SetVector3(diffuse, "pointLight.diffuse");
			shaderRef->SetVector3(specular, "pointLight.specular");

			constant = 1.0f;
			linear = 0.09f;
			quadratic = 0.032f;
			shaderRef->SetFloat(constant, "pointLight.constant");
			shaderRef->SetFloat(linear, "pointLight.linear");
			shaderRef->SetFloat(quadratic, "pointLight.quadratic");
		}
		if (ImGui::Button("reset to zero"))
		{
			ambient = glm::vec3(0, 0, 0);
			diffuse = glm::vec3(0, 0, 0);
			specular = glm::vec3(0, 0, 0);
			shaderRef->SetVector3(ambient, "pointLight.ambient");
			shaderRef->SetVector3(diffuse, "pointLight.diffuse");
			shaderRef->SetVector3(specular, "pointLight.specular");

			constant = 0.0f;
			linear = 0.0f;
			quadratic = 0.0f;
			shaderRef->SetFloat(constant, "pointLight.constant");
			shaderRef->SetFloat(linear, "pointLight.linear");
			shaderRef->SetFloat(quadratic, "pointLight.quadratic");
		}
		if (ImGui::DragFloat3("ambient", &ambient.x, .01f))
		{
			shaderRef->SetVector3(ambient, "pointLight.ambient");
		}
		if (ImGui::DragFloat3("diffuse", &diffuse.x, .01f))
		{
			shaderRef->SetVector3(diffuse, "pointLight.diffuse");
		}
		if (ImGui::DragFloat3("specular", &specular.x, .01f))
		{
			shaderRef->SetVector3(specular, "pointLight.specular");
		}

		if (ImGui::DragFloat("constant", &constant, .01f))
		{
			shaderRef->SetFloat(constant, "pointLight.constant");
		}
		if (ImGui::DragFloat("linear", &linear, .01f))
		{
			shaderRef->SetFloat(linear, "pointLight.linear");
		}
		if (ImGui::DragFloat("quadratic", &quadratic, .01f))
		{
			shaderRef->SetFloat(quadratic, "pointLight.quadratic");
		}
	}

	if (selectedType == 2) //spot light
	{
		if (ImGui::Button("reset to defaults"))
		{
			ambient = glm::vec3(0, 0, 0);
			diffuse = glm::vec3(1, 1, 1);
			specular = glm::vec3(1, 1, 1);
			shaderRef->SetVector3(ambient, "spotLight.ambient");
			shaderRef->SetVector3(diffuse, "spotLight.diffuse");
			shaderRef->SetVector3(specular, "spotLight.specular");

			constant = 1.0f;
			linear = 0.09f;
			quadratic = 0.032f;
			shaderRef->SetFloat(constant, "spotLight.constant");
			shaderRef->SetFloat(linear, "spotLight.linear");
			shaderRef->SetFloat(quadratic, "spotLight.quadratic");

			cutoff = glm::cos(glm::radians(12.5f)); 
			outerCutoff = glm::cos(glm::radians(17.5f)); 
			shaderRef->SetFloat(cutoff, "spotLight.cutoff");
			shaderRef->SetFloat(cutoff, "spotLight.outerCutoff");
		}
		if (ImGui::Button("reset to zero"))
		{
			ambient = glm::vec3(0, 0, 0);
			diffuse = glm::vec3(0, 0, 0);
			specular = glm::vec3(0, 0, 0);
			shaderRef->SetVector3(ambient, "spotLight.ambient");
			shaderRef->SetVector3(diffuse, "spotLight.diffuse");
			shaderRef->SetVector3(specular, "spotLight.specular");

			constant = 0.0f;
			linear = 0.0f;
			quadratic = 0.0f;
			shaderRef->SetFloat(constant, "spotLight.constant");
			shaderRef->SetFloat(linear, "spotLight.linear");
			shaderRef->SetFloat(quadratic, "spotLight.quadratic");

			cutoff = glm::cos(glm::radians(0.0f)); 
			outerCutoff = glm::cos(glm::radians(0.0f));
			shaderRef->SetFloat(cutoff, "spotLight.cutoff");
			shaderRef->SetFloat(cutoff, "spotLight.outerCutoff");
		}

		if (ImGui::DragFloat3("ambient", &ambient.x, .01f))
		{
			shaderRef->SetVector3(ambient, "spotLight.ambient");
		}
		if (ImGui::DragFloat3("diffuse", &diffuse.x, .01f))
		{
			shaderRef->SetVector3(diffuse, "spotLight.diffuse");
		}
		if (ImGui::DragFloat3("specular", &specular.x, .01f))
		{
			shaderRef->SetVector3(specular, "spotLight.specular");
		}

		if (ImGui::DragFloat("constant", &constant, .01f))
		{

			shaderRef->SetFloat(constant, "spotLight.constant");
		}
		if (ImGui::DragFloat("linear", &linear, .01f))
		{

			shaderRef->SetFloat(linear, "spotLight.linear");
		}
		if (ImGui::DragFloat("quadratic", &quadratic, .01f))
		{

			shaderRef->SetFloat(quadratic, "spotLight.quadratic");
		}

		if (ImGui::DragFloat("cutoff", &cutoff, .01f))
		{
			float myCutoff = glm::cos(glm::radians(cutoff)); //calculate cosine value before handing it to the fragment shader because it's an expensive operation and we don't wanna do that inside the shader.
			shaderRef->SetFloat(myCutoff, "spotLight.cutoff");
		}
		if (ImGui::DragFloat("outerCutoff", &outerCutoff, .01f))
		{
			float myOuterCutoff = glm::cos(glm::radians(outerCutoff)); //calculate cosine value before handing it to the fragment shader because it's an expensive operation and we don't wanna do that inside the shader.
			shaderRef->SetFloat(myOuterCutoff, "spotLight.outerCutoff");
		}
	}
}
