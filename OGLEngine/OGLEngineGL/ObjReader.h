#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "glm.hpp"
//#include "Mesh.h"
class Mesh; //Okay, I gotta read up on this, since doing a forward declaration of this here seems to fix error that I interpret as being a result of two classes #including one another. 

class ObjReader
{
public:
	bool parseOBJ(const std::string& filename, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);

	bool ReadFrom(std::iostream& file);

	bool WriteTo(std::iostream& file) const;

	void Serialization();

	void Deserialization();
private:
};

