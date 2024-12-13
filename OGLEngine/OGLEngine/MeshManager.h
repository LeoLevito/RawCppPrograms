#pragma once
#include <vector>
#include "Mesh.h"
class MeshManager
{
private:
	MeshManager();
	~MeshManager();
public:
	static MeshManager& Get();
	
	Mesh* LoadMesh(const std::string& filename); //include load from file or load from memory if requested mesh has been cached.
	//I have a question, how do I make it so multiple objects can have the same mesh while effectively only allocating and using memory for one mesh?
	void CacheMesh(); //isn't this done by adding it to the vector?

	//Should I just cache every mesh? No that's a bad idea.
	//but maybe I can just get an element of this vector and set the mesh to that?
	std::vector<Mesh*> meshes;
	std::vector<std::string> CachedMeshes; //(should I do this?) I don't think const is gonna work
	ObjReader* objreader;
private:
	void PrintMemoryStatus();
};

