#include "LightManager.h"

LightManager LightManager::Get()
{
	static LightManager instance;
	return instance;
}

Light* LightManager::AddNewLight(LightType type)
{
	switch (type)
	{
	case DirectionalLightType:
	{ //curious thing with switch cases, if you wanna initialize a new object, like directionalLight here, you need to have explicit scopes ({}), since otherwise we'd get an uninitialized variable if this case isn't hit. Best to just put this stuff in a function and call that instead. 
		DirectionalLight* directionalLight = new DirectionalLight();
		directionalLightVector.push_back(directionalLight);
		directionalLight->ID = directionalLightVector.size() - 1;
		return directionalLight;
	}
	case PointLightType:
	{
		PointLight* pointLight = new PointLight();
		pointLightVector.push_back(pointLight);
		pointLight->ID = pointLightVector.size() - 1;
		return pointLight;
	}
	case SpotLightType:
	{
		SpotLight* spotLight = new SpotLight();
		spotLightVector.push_back(spotLight);
		spotLight->ID = spotLightVector.size() - 1;
		return spotLight;
	}
	default:
		break;
	}
}

void LightManager::DeleteLight(LightType type, Light* light)
{
	switch (type)
	{
	case DirectionalLightType:
		directionalLightVector.erase(std::remove(directionalLightVector.begin(), directionalLightVector.end(), light));
		for (int i = 0; i < directionalLightVector.size(); i++)
		{
			directionalLightVector[i]->ID = i;
		}
		delete light;
		break;
	case PointLightType:
		pointLightVector.erase(std::remove(pointLightVector.begin(), pointLightVector.end(), light));
		for (int i = 0; i < pointLightVector.size(); i++)
		{
			pointLightVector[i]->ID = i;
		}
		delete light;
		break;
	case SpotLightType:
		spotLightVector.erase(std::remove(spotLightVector.begin(), spotLightVector.end(), light));
		for (int i = 0; i < spotLightVector.size(); i++)
		{
			spotLightVector[i]->ID = i;
		}
		delete light;
		break;
	default:
		break;
	}
}
