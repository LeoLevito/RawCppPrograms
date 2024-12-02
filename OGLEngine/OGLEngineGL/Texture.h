#pragma once
class Texture
{
public:
	Texture(const char* path);
	~Texture();
	int Width;
	int Height;
	
	unsigned int TextureObject;
};

