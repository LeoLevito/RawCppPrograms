#pragma once
class Texture
{
public:
	Texture(const char* path, int minfilter, int magfilter);
	~Texture();
	int Width;
	int Height;
	
	unsigned int TextureObject;
	//GLenum imageFormat;
};

