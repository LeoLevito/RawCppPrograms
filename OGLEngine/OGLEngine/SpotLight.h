#pragma once
#include "Light.h"
class SpotLight : public Light
{
public:
	SpotLight();

	glm::vec3 position;
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float linear;
	float quadratic;

	float cutoff;
	float outerCutoff;


	std::string IDstring;

	std::string positionString;
	std::string directionString;

	std::string ambientString;
	std::string diffuseString;
	std::string specularString;

	std::string linearString;
	std::string quadraticString;

	std::string cutoffString;
	std::string outerCutoffString;


	virtual void UpdateIDBasedStrings() override;

	virtual void SetToDefault() override;
	virtual void SetToCurrent() override;
	virtual void SetToZero(bool alsoSetVariables) override;

	virtual void SetPosition(glm::vec3 pos);
	virtual void SetDirection(glm::vec3 dir);

	virtual void DrawImgui() override;

	virtual void Serialization(std::fstream& file) override;
	virtual void Deserialization(std::fstream& file) override;
private:
};

