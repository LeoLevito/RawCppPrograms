#include "LightManager.h"

LightManager::LightManager()
{
	directionalLightVector.reserve(3 * sizeof(DirectionalLight*));
	pointLightVector.reserve(3 * sizeof(PointLight*));
	spotLightVector.reserve(3 * sizeof(SpotLight*));

}

LightManager::~LightManager()
{
}

LightManager& LightManager::Get()
{
	static LightManager instance;
	return instance;
}

Light* LightManager::AddNewLight(LightType type)
{
	switch (type)
	{
	case LightType::DirectionalLightType:
	{ //curious thing with switch cases, if you wanna initialize a new object, like directionalLight here, you need to have explicit scopes ({}), since otherwise we'd get an uninitialized variable if this case isn't hit. Best to just put this stuff in a function and call that instead. 
		DirectionalLight* directionalLight = new DirectionalLight();
		directionalLightVector.push_back(directionalLight);
		directionalLight->ID = directionalLightVector.size() - 1;
		directionalLight->UpdateIDBasedStrings();
		directionalLight->type = type;
		//directionalLight->SetToDefault();
		return directionalLight;
	}
	case LightType::PointLightType:
	{
		PointLight* pointLight = new PointLight();
		pointLightVector.push_back(pointLight);
		pointLight->ID = pointLightVector.size() - 1;
		pointLight->UpdateIDBasedStrings();
		pointLight->type = type;
		return pointLight;
	}
	case LightType::SpotLightType:
	{
		SpotLight* spotLight = new SpotLight();
		spotLightVector.push_back(spotLight);
		spotLight->ID = spotLightVector.size() - 1;
		spotLight->UpdateIDBasedStrings();
		spotLight->type = type;
		return spotLight;
	}
	default:
		break;
	}
}

void LightManager::DeleteLight(LightType type, Light* light)
{
	//mark for deletion?
	//NOTE: Shifting IDs causes a problem where lights may be swapped between light components. This causes confusion and should be worked out.

	light->SetToZero(false); //why doesn't this work when deleting entire game objects?
	switch (type)
	{
	case LightType::DirectionalLightType:
		directionalLightVector.erase(std::remove(directionalLightVector.begin(), directionalLightVector.end(), light));
		for (int i = 0; i < directionalLightVector.size(); i++)
		{
			directionalLightVector[i]->SetToZero(false);

			directionalLightVector[i]->ID = i; //this is where the ID shift happens on the previous light. I don't want this to happen when I manually set the ID's.
			directionalLightVector[i]->UpdateIDBasedStrings();

			directionalLightVector[i]->SetToCurrent();
		}
		break;
	case LightType::PointLightType:
		pointLightVector.erase(std::remove(pointLightVector.begin(), pointLightVector.end(), light));
		for (int i = 0; i < pointLightVector.size(); i++)
		{
			pointLightVector[i]->SetToZero(false);

			pointLightVector[i]->ID = i;
			pointLightVector[i]->UpdateIDBasedStrings();

			pointLightVector[i]->SetToCurrent();
		}
		break;
	case LightType::SpotLightType:
		spotLightVector.erase(std::remove(spotLightVector.begin(), spotLightVector.end(), light));
		for (int i = 0; i < spotLightVector.size(); i++)
		{
			spotLightVector[i]->SetToZero(false);

			spotLightVector[i]->ID = i;
			spotLightVector[i]->UpdateIDBasedStrings();

			spotLightVector[i]->SetToCurrent();
		}
		break;
	default:
		break;
	}
	delete light;
}

void LightManager::DrawImgui()
{
	if (ImGui::Checkbox("Blinn?", &blinn)) //this is a global toggle right?
	{
		if (ShaderManager::Get().depthPass == false)
		{
			ShaderManager::Get().shader->SetBool(blinn, "blinn");
		}
	}
}
