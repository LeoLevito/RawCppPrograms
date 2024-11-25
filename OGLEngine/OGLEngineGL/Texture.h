#pragma once
class Texture
{
public:
	Texture(const char* path);
	int Width;
	int Height;
	
	unsigned int TextureObject;
};

