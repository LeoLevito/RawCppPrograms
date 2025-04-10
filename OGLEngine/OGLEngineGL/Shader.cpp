#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <GLAD/glad.h> //bruh.
#include <gtc/type_ptr.hpp>


std::string Shader::LoadShader(const char* path) //pretty standard way of loading a file.
{
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try //how it's done on learnopengl.com/Getting-started/Shaders.
    {
        shaderFile.open(path);

        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();

        shaderFile.close();
        shaderCode = shaderStream.str();

        return shaderCode;
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Failed to load shader file from path: " << path << "\n";
        return "";
    }
}

unsigned int Shader::LoadVertexShader(const char* path) //create vertex shader object.
{
    int result;
    char Log[512];

    std::string shaderCodeString = LoadShader(path); 
    const char* shaderCode = shaderCodeString.c_str(); //convert loaded shader code to char pointer.

    unsigned int shaderObject; //actual shader object

    shaderObject = glCreateShader(GL_VERTEX_SHADER); //create shader object
    glShaderSource(shaderObject, 1, &shaderCode, NULL); //...
    glCompileShader(shaderObject); //compile shader object

    //check if shader compiles or not. Get a log from this.
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(shaderObject, 512, NULL, Log);
        std::cout << "Failed to compile vertex shader \n" << Log << std::endl;
    }

    return shaderObject;
}

unsigned int Shader::LoadFragmentShader(const char* path)
{
    int result;
    char Log[512];

    std::string shaderCodeString = LoadShader(path);
    const char* shaderCode = shaderCodeString.c_str();

    unsigned int shaderObject;

    shaderObject = glCreateShader(GL_FRAGMENT_SHADER); //only difference is GL_FRAGMENT_SHADER
    glShaderSource(shaderObject, 1, &shaderCode, NULL);
    glCompileShader(shaderObject);

    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(shaderObject, 512, NULL, Log);
        std::cout << "Failed to compile vertex shader \n" << Log << std::endl;
    }
    return shaderObject;
}

unsigned int Shader::LoadGeometryShader(const char* path)
{
    int result;
    char Log[512];

    std::string shaderCodeString = LoadShader(path);
    const char* shaderCode = shaderCodeString.c_str();

    unsigned int shaderObject;

    shaderObject = glCreateShader(GL_GEOMETRY_SHADER); //only difference is GL_GEOMETRY_SHADER, let's hope this works.
    glShaderSource(shaderObject, 1, &shaderCode, NULL);
    glCompileShader(shaderObject);

    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(shaderObject, 512, NULL, Log);
        std::cout << "Failed to compile vertex shader \n" << Log << std::endl;
    }
    return shaderObject;
}

void Shader::Initialize(const char* vertexPath, const char* fragmentPath, const char* geometryPath) //start shader program.
{
    int result;
    char Log[512];

    unsigned int VertexShader = LoadVertexShader(vertexPath); //NEED SHADER FILES TO BE INSIDE THE FOLDER, IN EXPLORER, WHERE THE CURRENT PROJECT RUNS. IN THIS CASE: INSIDE OGLENGINE FOLDER IN EXPLORER. https://github.com/JoeyDeVries/LearnOpenGL/issues/146#issuecomment-528748296
    unsigned int FragmentShader = LoadFragmentShader(fragmentPath);
    unsigned int GeometryShader;
    if (geometryPath != nullptr)
    {
        GeometryShader = LoadGeometryShader(geometryPath);
    }

    myShaderProgram = glCreateProgram(); //we are not deleting any shader programs when deleting objects, so that's something to keep in mind.
    glAttachShader(myShaderProgram, VertexShader);
    glAttachShader(myShaderProgram, FragmentShader);
    if (geometryPath != nullptr)
    {
        glAttachShader(myShaderProgram, GeometryShader);
    }
    glLinkProgram(myShaderProgram); //might get conflicts here if vertex and fragment shaders have different input and outputs, or whatever. This function creates an executable that will be run on the programmable vertex processor if any shader objects here are of type GL_VERTEX_SHADER.

    //check if program compiles or not. Get a log from this.
    glGetProgramiv(myShaderProgram, GL_LINK_STATUS, &result);
    if (!result) 
    {
        glGetProgramInfoLog(myShaderProgram, 512, NULL, Log);
        std::cout << "Shader program could not compile\n" << Log << std::endl;
    }

    //we don't need the shader objects anymore since we now have our shader program!
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    if (geometryPath != nullptr)
    {
        glDeleteShader(GeometryShader);
    }

    //NOTE: Martin mentioned that he did this function weirdly since it doesn't return anything, and that it creates two shader objects now.
}

void Shader::Use() //think of the state machine again, set the current state's shader program to this one.
{
    glUseProgram(myShaderProgram); 
}

void Shader::SetMatrix4(glm::mat4& matrix, const std::string& name) //Set Matrix 4 uniform variables inside vertex shader during runtime.
{
    //int location = glGetUniformLocation(myShaderProgram, name.c_str());
    //std::cout << "location: " << location << " name: " << name << " this is my location, if this is -1 it's invalid!!!" << std::endl;
    glUniformMatrix4fv(glGetUniformLocation(myShaderProgram, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetVector2(glm::vec2 vector2, const std::string& name)
{
    glUniform2fv(glGetUniformLocation(myShaderProgram, name.c_str()), 1, &vector2[0]);
}

void Shader::SetVector3(glm::vec3 vector3, const std::string& name)
{
    glUniform3fv(glGetUniformLocation(myShaderProgram, name.c_str()), 1, &vector3[0]);
}

void Shader::SetVector4(glm::vec4 vector4, const std::string& name)
{
    glUniform4fv(glGetUniformLocation(myShaderProgram, name.c_str()), 1, &vector4[0]);
}

void Shader::SetFloat(float value, const std::string& name)
{
    glUniform1f(glGetUniformLocation(myShaderProgram, name.c_str()), value);
}

void Shader::SetInt(int value, const std::string& name)
{
    //int location = glGetUniformLocation(myShaderProgram, name.c_str());
    //std::cout << "location: " << location << " name: " << name << " this is my location, if this is -1 it's invalid!!!" << std::endl;
    glUniform1i(glGetUniformLocation(myShaderProgram, name.c_str()), value);
}

void Shader::SetBool(bool value, const std::string& name)
{
    glUniform1i(glGetUniformLocation(myShaderProgram, name.c_str()), (int)value);
}


