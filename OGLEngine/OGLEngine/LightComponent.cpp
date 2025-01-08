#include "LightComponent.h"
#include <GameObject.h>
#include <TransformComponent.h>
#include <iostream>

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
	for (size_t i = 0; i < owner->components.size(); i++)
	{
		if (dynamic_cast<TransformComponent*>(owner->components[i])) //checking if owner has a component of type TransformComponent. Is of-type correct word-use in this case?
		{
			//multiple casts, in UPDATE()? not sure how efficient this is. EDIT 5 dec 2024, apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times.
			position = dynamic_cast<TransformComponent*>(owner->components[i])->position; //need to get a reference to the transform component if there is one.
			rotation = dynamic_cast<TransformComponent*>(owner->components[i])->rotation;
			scale = dynamic_cast<TransformComponent*>(owner->components[i])->scale;
		}
	}

	shader->SetVector3(position, "lightPosition"); //Light position will be kept when this component is deleted, because it was last updated here.
}

void LightComponent::DrawComponentSpecificImGuiHierarchyAdjustables()
{
	Component::DrawComponentSpecificImGuiHierarchyAdjustables();
}
