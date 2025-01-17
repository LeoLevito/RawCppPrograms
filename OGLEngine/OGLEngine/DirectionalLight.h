#pragma once
#include "Light.h"
class DirectionalLight : public Light
{
public:
	DirectionalLight();

	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;


	std::string IDstring;

	std::string directionString;

	std::string ambientString;
	std::string diffuseString;
	std::string specularString;


	virtual void UpdateIDBasedStrings() override;

	virtual void SetToDefault() override;
	virtual void SetToCurrent() override;
	virtual void SetToZero() override;

	virtual void SetDirection(glm::vec3 dir) override;

	virtual void DrawImgui() override;
private:
};

