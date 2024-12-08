#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "glm.hpp"
#include "vboindexer.h"
//#include "Mesh.h"
class Mesh; //Okay, I gotta read up on this, since doing a forward declaration of this here seems to fix error that I interpret as being a result of two classes #including one another. 

class ObjReader
{
public:
	ObjReader();

	bool parseOBJ(const std::string& filename, std::vector<unsigned short>& indices, std::vector<glm::vec3>& indexed_vertices, std::vector<glm::vec2>& indexed_uvs, std::vector<glm::vec3>& indexed_normals);

	bool ReadFrom(std::iostream& file);

	bool WriteTo(std::iostream& file) const;

	void Serialization(const std::string& filename);

	void Deserialization(const std::string& filename);

	vboindexer* myVBOindexer;

	std::vector<unsigned short> myIndices;
	std::vector<glm::vec3> myIndexed_vertices;
	std::vector<glm::vec2> myIndexed_uvs;
	std::vector<glm::vec3> myIndexed_normals;
private:
};

