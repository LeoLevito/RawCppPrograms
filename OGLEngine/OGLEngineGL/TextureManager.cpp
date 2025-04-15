#include "TextureManager.h"

TextureManager::TextureManager()
{
	path = "../Textures/";
	for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path))
	{
		texturePaths.push_back(entry);
	}
	//RefreshTexturePreviews();
	LoadTexturePreviews();
}

TextureManager::~TextureManager()
{
}

TextureManager& TextureManager::Get()
{
	static TextureManager instance;
	return instance;
}

void TextureManager::RefreshTexturePreviews()
{
	for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path))
	{
		texturePaths.push_back(entry);
	}

	for (int i = 0; i < texturePaths.size(); i++)
	{
		if (texturePaths[i].path().has_extension()) //if does have extension
		{
			if (texturePaths[i].path().string().find("_preview.jpg") == std::string::npos) //if we don't find "_preview.jpg" in the path name.
			{
				Texture preview(texturePaths[i].path().string().c_str(), 5, 1, true);
			}
		}
	}
}

void TextureManager::LoadTexturePreviews()
{
	for (int i = 0; i < texturePaths.size(); i++)
	{
		if (texturePaths[i].path().has_extension()) //if does have extension
		{
			if (texturePaths[i].path().string().find("_preview.jpg") != std::string::npos) //if we find "_preview.jpg" in the path name.
			{
				Texture* texture = new Texture(texturePaths[i].path().string().c_str(), 5, 1, false);
				texturePreviews.push_back(texture); 
			}
		}
	}
}


//Maybe I can write smaller versions of the loaded texture images to disk using stb_image_write.h (jpg is possible),
//I would need a way to downscale the texture images to 32x32, either using some function in stb,
//or using imgui like I do to display the images in the MeshComponent texture selection menu.