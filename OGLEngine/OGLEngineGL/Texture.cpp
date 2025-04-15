#include "Texture.h"
#include "stb_image.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h> //GLAD ALWAYS COMES BEFORE GLFW!!!
#include <iostream>
#include "stb_image_write.h"
#include "stb_image_resize2.h"
#include <filesystem>



Texture::Texture(const char* path, int minfilter, int magfilter, bool writePreview)
{
	name = path;
	if (name.find(".jpg") == std::string::npos) //if ".jpg" doesn't exist at any position in the string
	{
		return;
	}

	int Channels = 0;
	Width = 0; //set in stbi_load.
	Height = 0; //set in stbi_load.

	stbi_set_flip_vertically_on_load(true); //mirror texture vertically before loading it because OpenGL is weird. //https://learnopengl.com/index.php?p=Getting-started/Textures
	unsigned char* data = stbi_load(path, &Width, &Height, &Channels, 0); //load in texture from path using stb_image header.

	std::cout << "how many channels in texture? Answer: " << Channels << std::endl;
	glGenTextures(1, &TextureObject); //similar to what we do in Mesh constructor with the VBO and VAO, we have to generate the TextureObject and bind it.
	glBindTexture(GL_TEXTURE_2D, TextureObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //repeat texture in y direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //repeat texture in x direction

	switch (minfilter)
	{
	case 0:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	case 1:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case 2:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		break;
	case 3:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		break;
	case 4:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		break;
	case 5:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	default:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //mipmapping setup, minifying function, apply what texture filter to use in the last argument.
		break;
	}
	switch (magfilter)
	{
	case 0:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case 1:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	default:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //mipmapping setup, magnification function, apply what texture filter to use in the last argument.
		break;
	}

	if (data)
	{
		//example if channels = 1 or similar, change GL_RGB to GL_RG or similar.
		//											    here.
		int imageFormat = GL_RGB;
		switch (Channels)
		{
		case 0:
			std::cout << "Failed to load texture, no channels in image. " << std::endl;
			return;
		case 1:
			imageFormat = GL_RED;

			//map red channel to rest of channels;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
			//Alpha channel?

			break;
		case 2:
			imageFormat = GL_RG;
			break;
		case 3:
			imageFormat = GL_RGB;
			break;
		case 4:
			imageFormat = GL_RGBA;
			break;
		default:
			break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, imageFormat, GL_UNSIGNED_BYTE, data); //bind texture that was loaded from path after parameters have been set. (data). NOTE: GL_RGB doesn't support images with a bit depth of 8. So be sure to re-save your images for 24, 32, 48 and maybe even 16 .
		glGenerateMipmap(GL_TEXTURE_2D); //Generate mipmaps
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl; //if texture couldn't be loaded (load-function returned null), we output failure message to console.
	}

	glBindTexture(GL_TEXTURE_2D, 0); //unbind when we're done with the setup.




	if (writePreview)
	{
		int pos = std::string(path).find("\\");
		if (std::string(path).find("_preview.jpg") == std::string::npos) //extra check for if "_preview.jpg" doesn't exist in the string name
		{

			std::string newDirectoryName = path;
			newDirectoryName.erase(pos, newDirectoryName.length());
			newDirectoryName.append("/_previews");

			std::filesystem::create_directory(newDirectoryName);

			std::string fileName = path;
			fileName.erase(0, pos);
			fileName.erase(fileName.length() - 4); //remove ".jpg"
			fileName.append("_preview.jpg"); //Add "_preview.jpg"

			std::string newName;
			newName.append(newDirectoryName);
			newName.append(fileName); 

			unsigned char* data2 = stbir_resize_uint8_srgb(data, Width, Height, 0, 0, 32, 32, 0, stbir_pixel_layout::STBIR_RGB);
			stbi_flip_vertically_on_write(true);
			stbi_write_jpg(newName.c_str(), 32, 32, Channels, data2, 100);
		}
	}

	stbi_image_free(data); //free up data when we're done.
}

Texture::~Texture()
{
	glDeleteTextures(1, &TextureObject);
}
