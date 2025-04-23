#include "TextureManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

TextureManager::TextureManager()
{
	GetTexturePaths();
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

void TextureManager::GetTexturePaths()
{
	texturePaths.clear();
	path = "../Textures/";
	for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path))
	{
		std::string test = entry.path().string();
		if (!entry.path().has_extension() && test.find("_system") != std::string::npos) //exclude that directory.
		{
			continue;
		}
		else if (entry.path().has_extension() && test.find("_system") != std::string::npos) //exclude files in that directory.
		{
			continue;
		}

		texturePaths.push_back(entry);
	}
}

void TextureManager::RefreshTexturePreviews()
{
	//for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path))
	//{
	//	texturePaths.push_back(entry);
	//}
	for (int i = 0; i < texturePaths.size(); i++)
	{
		if (texturePaths[i].path().has_extension()) //if does have extension
		{
			if (texturePaths[i].path().string().find("_preview.jpg") == std::string::npos && texturePaths[i].path().string().find(".jpg.bin") == std::string::npos) //if we don't find "_preview.jpg" in the path name.
			{
				Texture preview(texturePaths[i].path().string().c_str(), 5, 1, true, false);
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
				Texture* texture = new Texture(texturePaths[i].path().string().c_str(), 5, 1, false, true);
				texturePreviews.push_back(texture); 
			}

			//if (texturePaths[i].path().string().find(".jpg.bin") != std::string::npos) //if we find "_preview.jpg" in the path name.
			//{
			//	Texture* texture = new Texture(texturePaths[i].path().string().c_str(), 5, 1, false);
			//	textures.push_back(texture);
			//}
		}
	}
	int i;
}

void TextureManager::DrawImgui()
{
	ImGui::BeginDisabled(disableRefreshButton == true);
	if (ImGui::Button("Refresh texture previews & bins"))
	{
		
		GetTexturePaths();
		RefreshTexturePreviews();
		GetTexturePaths();
		disableRefreshButton = true;
	}
	ImGui::EndDisabled();
	if (disableRefreshButton == true)
	{
		ImGui::SameLine();
		ImGui::Text("Done... Took a while.");
	}
}


//Maybe I can write smaller versions of the loaded texture images to disk using stb_image_write.h (jpg is possible),
//I would need a way to downscale the texture images to 32x32, either using some function in stb,
//or using imgui like I do to display the images in the MeshComponent texture selection menu.