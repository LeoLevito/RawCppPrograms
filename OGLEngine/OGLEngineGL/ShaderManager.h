#pragma once
#include "Shader.h"
#include "ShadowMap.h"
#include "ShadowCubeMap.h"

class ShaderManager
{
private:
	ShaderManager();
	~ShaderManager();

public:
	void Initialize();
	static ShaderManager& Get();
	Shader* shader;
	Shader* depthShader;
	Shader* depthCubeMapShader;
	Shader* lineShader;	
	Shader* gridShader;
	ShadowMap* shadowMap;
	ShadowCubeMap* shadowCubeMap;
	bool depthPass;
};

