#pragma once
#include "Light.h"
class DirectionalLight : public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

	glm::vec3 position;
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;


	std::string IDstring;

	std::string positionString;
	std::string directionString;

	std::string ambientString;
	std::string diffuseString;
	std::string specularString;

	virtual void UpdateIDBasedStrings() override;

	virtual void SetToDefault() override;
	virtual void SetToCurrent() override;
	virtual void SetToZero() override;

	virtual void SetPosition(glm::vec3 pos) override;
	virtual void SetDirection(glm::vec3 dir) override;
	virtual void SetLightSpaceMatrix() override;

	virtual void DrawImgui() override;

	virtual void Serialization(std::fstream& file) override;
	virtual void Deserialization(std::fstream& file) override;
private:
};

