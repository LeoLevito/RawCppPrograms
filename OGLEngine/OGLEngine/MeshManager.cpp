#include "MeshManager.h"
#include <iostream>
#include <windows.h>


MeshManager::MeshManager()
{
	objreader = new ObjReader;
}

MeshManager::~MeshManager()
{
	delete objreader;
}

MeshManager& MeshManager::Get()
{
	static MeshManager instance; //so this will be instanciated once and then on subsequent calls of this Get() function it will just be referenced! See https://youtu.be/PPup1yeU45I?t=888 for more info.
	return instance;
}

Mesh* MeshManager::LoadMesh(const std::string& filename)
{
	for (size_t i = 0; i < CachedMeshes.size(); i++) //why doesn't CachedMeshes know what it has? It just says error can't read string or whatever?
	{
		if (CachedMeshes[i] == filename) { //since ImGui entry doesn't require proper capitalization, there can be situations where we load the same mesh multiple times when we don't need to. This would be fixed by having a dropdown to choose meshes from instead of a string entry.
			return meshes[i];
		}
	}

	PrintMemoryStatus();
	Mesh* mesh = new Mesh(objreader, filename);
	PrintMemoryStatus();

	meshes.push_back(mesh);
	CachedMeshes.push_back(filename);
	return mesh;

}

void MeshManager::PrintMemoryStatus()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	std::cout << "There is " << statex.ullAvailPhys / (1024 * 1024) << " MB of physical memory available." << std::endl;
	std::cout << "There is " << statex.ullAvailVirtual / (1024 * 1024) << " MB of virtual memory available." << std::endl;
}
