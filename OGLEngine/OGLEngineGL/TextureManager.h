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
	void RefreshTexturePreviews(); 
	void LoadTexturePreviews();

	std::string path;
	std::vector<Texture*> textures;
	std::vector<Texture*> texturePreviews;
	std::vector<std::filesystem::directory_entry> texturePaths;
private:
};

