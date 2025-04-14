#pragma once
#include <vector>
#include "Texture.h"
#include <filesystem>
class TextureManager
{
private:
	TextureManager();
	~TextureManager();
public:
	static TextureManager& Get();
	void ReloadSpecifiedTexture(int i, int minType, int magType);

	std::vector<Texture*> textures;
	std::vector<std::filesystem::directory_entry> texturePaths;
private:
};

