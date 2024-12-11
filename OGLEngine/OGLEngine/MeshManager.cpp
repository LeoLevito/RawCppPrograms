#include "MeshManager.h"



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
	for (size_t i = 0; i < CachedMeshes.size(); i++) //why coesn't CachedMeshes know what it has? It just says error can't read string or whatever?
	{
		if (CachedMeshes[i] == &filename) {
			return meshes[i];
		}
	}
	Mesh* mesh = new Mesh(objreader, filename);
	meshes.push_back(mesh);
	CachedMeshes.push_back(&filename);
	return mesh;
}
