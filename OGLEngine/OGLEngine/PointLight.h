#pragma once
#include "Light.h"
class PointLight : public Light
{
public:
	PointLight();

	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float linear;
	float quadratic;


	std::string IDstring;

	std::string positionString;

	std::string ambientString;
	std::string diffuseString;
	std::string specularString;

	std::string linearString;
	std::string quadraticString;


	virtual void UpdateIDBasedStrings() override;

	virtual void SetToDefault() override;
	virtual void SetToCurrent() override;
	virtual void SetToZero() override;

	virtual void SetPosition(glm::vec3 pos);
	virtual void SetLightSpaceMatrix() override;

	virtual void DrawImgui() override;

	virtual void Serialization(std::fstream& file) override;
	virtual void Deserialization(std::fstream& file) override;
private:
};

