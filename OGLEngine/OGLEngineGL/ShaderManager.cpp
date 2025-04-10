#include "ShaderManager.h"

ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::Initialize()
{
	shader = new Shader(); //quick fix for the 0xCCCCCCCC write/read access violation.
	shader->Initialize("../Shaders/VertexShader.vertexs", "../Shaders/FragmentShader.fragments");
	depthShader = new Shader();
	depthShader->Initialize("../Shaders/SimpleDepthShader.vertexs", "../Shaders/SimpleDepthShader.fragments");
	depthCubeMapShader = new Shader();
	depthCubeMapShader->Initialize("../Shaders/SimpleDepthCubeMapShader.vertexs", "../Shaders/SimpleDepthCubeMapShader.fragments", "../Shaders/SimpleDepthCubeMapShader.geometrys");
	lineShader = new Shader();
	lineShader->Initialize("../Shaders/LineShader.vertexs", "../Shaders/LineShader.fragments");
	gridShader = new Shader();
	gridShader->Initialize("../Shaders/GridShader.vertexs", "../Shaders/GridShader.fragments");
	pickingShader = new Shader();
	pickingShader->Initialize("../Shaders/PickingShader.vertexs", "../Shaders/PickingShader.fragments");
	outlineShader = new Shader();
	outlineShader->Initialize("../Shaders/OutlineShader.vertexs", "../Shaders/OutlineShader.fragments");
	outlineActualShader = new Shader();
	outlineActualShader->Initialize("../Shaders/OutlineActualShader.vertexs", "../Shaders/OutlineActualShader.fragments");
	shadowMap = new ShadowMap();
	shadowCubeMap = new ShadowCubeMap();
	depthPass = false;
	pickingPass = false;
	outlinePass = false;
}

ShaderManager& ShaderManager::Get()
{
	static ShaderManager instance;
	return instance;
}
