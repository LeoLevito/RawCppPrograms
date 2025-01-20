#pragma once
#include "glm.hpp"
#include <string> 
#include "ShaderManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//#include "LightManager.h"

//class LightManager;

enum class LightType
{
	DirectionalLightType,
	PointLightType,
	SpotLightType
};

class Light
{
public:
	Light();
	virtual ~Light();
	int ID;
	LightType type;

	virtual void UpdateIDBasedStrings();
	virtual void SetToDefault();
	virtual void SetToCurrent();
	virtual void SetToZero();

	virtual void SetPosition(glm::vec3 pos);
	virtual void SetDirection(glm::vec3 dir);

	virtual void DrawImgui();

	bool blinn;
private:
};

