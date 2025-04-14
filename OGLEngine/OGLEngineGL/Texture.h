#pragma once
#include <string>
class Texture
{
public:
	Texture(const char* path, int minfilter, int magfilter);
	~Texture();
	int Width;
	int Height;
	
	std::string name;
	unsigned int TextureObject;
	//GLenum imageFormat;
};

