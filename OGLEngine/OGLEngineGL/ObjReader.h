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

	struct ObjData
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> vertexNormals;
		std::vector<glm::vec2> vertexTexCoords;
		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices; //f stands for face in .obj file. It handles indices.
	};
	ObjData parseOBJ(const std::string& filename);
	Mesh* LoadObjMesh(const std::string& filename);

	bool Hello();
private:
};

