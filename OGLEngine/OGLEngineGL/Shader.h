#pragma once
#include <string>
class Shader
{
private:
	std::string LoadShader(const char* path);
	unsigned int LoadVertexShader(const char* path);
	unsigned int LoadFragmentShader(const char* path);
public:

	unsigned int myShaderProgram;
	void Initialize(const char* vertexPath, const char* fragmentPath);
	void Use();
};

