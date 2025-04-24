#pragma once
#include <string>
class Texture
{
public:
	Texture(const char* path, int minfilter, int magfilter, bool writePreview, bool readPreview, bool transparent = false); //Default parameters are cool!
	~Texture();
	int Width;
	int Height;
	int Channels;
	
	std::string name;
	unsigned int TextureObject;
	//GLenum imageFormat;


	void Serialization(const std::string& filename);
	void Deserialization(const std::string& filename);

	std::string masterstring;
};

