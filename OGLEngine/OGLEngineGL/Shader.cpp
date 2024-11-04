#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <GLAD/glad.h> //bruh.

std::string Shader::LoadShader(const char* path) //pretty standard way of loading a file.
{
    std::string shaderCode;
    std::ifstream shaderFile;

    shaderFile.open(path);

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();

    shaderFile.close();
    shaderCode = shaderStream.str();

    return shaderCode;
}

unsigned int Shader::LoadVertexShader(const char* path) //create vertex shader object.
{
    int result;
    char Log[512];


    std::string shaderCodeString = LoadShader(path); 
    //I COULDN'T GET THE CHAR* CONVERSION MARTIN DID TO WORK SO I JUST PUT IN THE GLSL SHADER CODE DIRECTLY:
    const char* shaderCode = "#version 330 core\nlayout(location = 0) in vec3 aPos;\nlayout(location = 1) in vec3 aColor;\nout vec4 vertexColor;\nvoid main()\n{\ngl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\nvertexColor = vec4(aColor, 1.0);\n}\0"/* shaderCodeString.c_str()*/; //convert loaded shader code to char pointer.

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
    //I COULDN'T GET THE CHAR* CONVERSION MARTIN DID TO WORK SO I JUST PUT IN THE GLSL SHADER CODE DIRECTLY:
    const char* shaderCode = "#version 330 core\nout vec4 FragColor;\nin vec4 vertexColor;\nvoid main()\n{\nFragColor = vertexColor;\n}\0"/*shaderCodeString.c_str()*/;

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

void Shader::Initialize(const char* vertexPath, const char* fragmentPath) //start shader program.
{
    int result;
    char Log[512];

    unsigned int VertexShader = LoadVertexShader(vertexPath);
    unsigned int FragmentShader = LoadFragmentShader(fragmentPath);

    myShaderProgram = glCreateProgram();
    glAttachShader(myShaderProgram, VertexShader);
    glAttachShader(myShaderProgram, FragmentShader);
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

    //NOTE: Martin mentioned that he did this function weirdly since it doesn't return anything, and that it creates two shader objects now.
}

void Shader::Use() //think of the state machine again, set the current state's shader program to this one.
{
    glUseProgram(myShaderProgram); 
}

void Shader::SetMatrix4(glm::mat4 matrix, const std::string& name) //missing glm math library, need to download that.
{
    glUniformMatrix4fv(glGetUniformLocation(myShaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ExampleCube() //put this in Graphics. Currently this is orthographic.
{
    for (glm::vec3 v : myCubePositions)
    {
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 1.0f));

        trans = glm::translate(trans, v);
        myShader->SetMatrix4(trans, "transform");

        myCube->Draw(myShader);
    }
}
