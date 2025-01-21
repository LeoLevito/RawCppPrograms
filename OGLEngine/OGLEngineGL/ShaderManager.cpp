#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	shader = new Shader(); //quick fix for the 0xCCCCCCCC write/read access violation.
	shader->Initialize("../Shaders/VertexShader.vertexs", "../Shaders/FragmentShader.fragments");
	depthShader = new Shader();
	depthShader->Initialize("../Shaders/SimpleDepthShader.vertexs", "../Shaders/SimpleDepthShader.fragments");
	shadowMap = new ShadowMap();
	depthPass = false;
}

ShaderManager::~ShaderManager()
{
}

ShaderManager& ShaderManager::Get()
{
	static ShaderManager instance;
	return instance;
}
