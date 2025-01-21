#pragma once
#include "Shader.h"
#include "ShadowMap.h"

class ShaderManager
{
private:
	ShaderManager();
	~ShaderManager();
public:
	static ShaderManager& Get();
	Shader* shader;
	Shader* depthShader;
	ShadowMap* shadowMap;
	bool depthPass;
};

