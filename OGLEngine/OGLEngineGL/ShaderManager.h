#pragma once
#include "Shader.h"
class ShaderManager
{
private:
	ShaderManager();
	~ShaderManager();
public:
	static ShaderManager& Get();
	Shader* shader;
};

