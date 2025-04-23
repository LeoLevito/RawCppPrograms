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
	void GetTexturePaths();
	void RefreshTexturePreviews(); 
	void LoadTexturePreviews();
	void DrawImgui();

	std::string path;
	std::vector<Texture*> textures;
	std::vector<Texture*> texturePreviews;
	std::vector<std::filesystem::directory_entry> texturePaths;

	bool disableRefreshButton = false;
private:
};

