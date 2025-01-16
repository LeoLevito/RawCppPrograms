#pragma once
#include "glm.hpp"
//class LightManager;

class Light
{
public:
	Light();
	virtual ~Light();
	int ID;
	//LightType type;

	virtual void UpdateIDBasedStrings();
	virtual void SetToDefault();
	virtual void SetToCurrent();
	virtual void SetToZero();


	virtual void DrawImgui();


private:
};

