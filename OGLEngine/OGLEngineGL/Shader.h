#pragma once
#include <string>
#include <glm.hpp>

class Shader
{
private:
	std::string LoadShader(const char* path);
	unsigned int LoadVertexShader(const char* path);
	unsigned int LoadFragmentShader(const char* path);
	unsigned int LoadGeometryShader(const char* path);
public:
	unsigned int myShaderProgram;
	void Initialize(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void Use();
	void SetMatrix4(glm::mat4& matrix, const std::string& name);
	void SetVector2(glm::vec2 vector2, const std::string& name);
	void SetVector3(glm::vec3 vector3, const std::string& name);
	void SetVector4(glm::vec4 vector4, const std::string& name);
	void SetFloat(float value, const std::string& name);
	void SetInt(int value, const std::string& name);
	void SetBool(bool value, const std::string& name);
};

