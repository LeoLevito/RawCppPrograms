#pragma once
#include <vector>
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"


class LightManager
{
public:
	static LightManager& Get();

	Light* AddNewLight(LightType type);
	void DeleteLight(LightType type, Light* light);

	std::vector<DirectionalLight*> directionalLightVector;
	std::vector<PointLight*> pointLightVector;
	std::vector<SpotLight*> spotLightVector;
private:
};

