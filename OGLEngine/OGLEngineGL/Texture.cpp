#include "Texture.h"
#include "stb_image.h"
#include <GLAD/glad.h>
#include <glfw3.h> //GLAD ALWAYS COMES BEFORE GLFW!!!
#include <iostream>


Texture::Texture(const char* path)
{
	int Channels = 0;
	Width = 0;
	Height = 0;

	unsigned char* data = stbi_load(path, &Width, &Height, &Channels, 0); //load in texture from path using stb_image header.

	glGenTextures(1, &TextureObject); //similar to what we do in Mesh constructor with the VBO and VAO, we have to generate the TextureObject and bind it.
	glBindTexture(GL_TEXTURE, TextureObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //repeat texture in y direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //repeat texture in x direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //mipmapping setup, minifying function, apply what texture filter to use in the last argument.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //mipmapping setup, magnification function, apply what texture filter to use in the last argument.

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //bind texture that was loaded from path after parameters have been set. (data).
		glGenerateMipmap(GL_TEXTURE_2D); //Generate mipmaps
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl; //if texture couldn't be loaded (load-function returned null), we output failure message to console.
	}

	stbi_image_free(data); //free up data when we're done.
	glBindTexture(GL_TEXTURE, 0); //unbind when we're done with the setup.
}
