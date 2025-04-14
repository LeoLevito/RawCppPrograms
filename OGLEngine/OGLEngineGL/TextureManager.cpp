#include "TextureManager.h"

TextureManager::TextureManager()
{
	std::string path = "../Textures/";
	for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path))
	{
		texturePaths.push_back(entry);
	}

	for (int i = 0; i < texturePaths.size(); i++)
	{
		if (texturePaths[i].path().has_extension()) //if does have extension
		{
			Texture* texture = new Texture(texturePaths[i].path().string().c_str(), 5, 1);
			textures.push_back(texture);
		}
		else
		{
			Texture* texture = new Texture(texturePaths[i].path().string().c_str(), 5, 1);
			textures.push_back(texture);
		}
	}

	int rama = 0;
}

TextureManager::~TextureManager()
{
}

TextureManager& TextureManager::Get()
{
	static TextureManager instance;
	return instance;
}

void TextureManager::ReloadSpecifiedTexture(int i, int minType, int magType)
{
	Texture* texture = new Texture(texturePaths[i].path().string().c_str(), minType, magType);
	textures[i] = texture;
}


//Maybe I can write smaller versions of the loaded texture images to disk using stb_image_write.h (jpg is possible),
//I would need a way to downscale the texture images to 32x32, either using some function in stb,
//or using imgui like I do to display the images in the MeshComponent texture selection menu.