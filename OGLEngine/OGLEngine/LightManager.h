#pragma once
#include <vector>
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

enum LightType
{
	DirectionalLightType,
	PointLightType,
	SpotLightType
};

class LightManager
{
public:
	static LightManager Get();



	Light* AddNewLight(LightType type);
	void DeleteLight(LightType type, Light* light);

	std::vector<DirectionalLight*> directionalLightVector;
	std::vector<PointLight*> pointLightVector;
	std::vector<SpotLight*> spotLightVector;
private:
};

