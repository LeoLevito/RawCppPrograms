#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <string> 
#include "ShaderManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <vector> 
#include <fstream>
#include <sstream>

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
	virtual void SetToZero(bool alsoSetVariables);

	virtual void SetPosition(glm::vec3 pos);
	virtual void SetDirection(glm::vec3 dir);
	virtual void SetLightSpaceMatrix();

	virtual void DrawImgui();

	virtual void Serialization(std::fstream& file);
	virtual void Deserialization(std::fstream& file);


private:
};

