#include "ObjReader.h"
#include <iostream>
#include "Mesh.h"



ObjReader::ObjData ObjReader::parseOBJ(const std::string& filename)
{
	ObjData objdata;
	std::ifstream file(filename);
	std::string line;

	if (!file.is_open())
	{
		std::cerr << "Failed to open .obj file: " << filename << std::endl;
		return objdata; //diff
	}

	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix; //what this do?

		if (prefix == "v")
		{
			glm::vec3 vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;
			//std::cout << "v: " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
			objdata.vertices.push_back(vertex);
		}
		else if (prefix == "vn")
		{
			glm::vec3 vertexNormal;
			iss >> vertexNormal.x >> vertexNormal.y >> vertexNormal.z;
			//std::cout << "vn: " << vertexNormal.x << " " << vertexNormal.y << " " << vertexNormal.z << std::endl;
			objdata.vertexNormals.push_back(vertexNormal);
		}
		else if (prefix == "vt")
		{
			glm::vec2 texCoord;
			iss >> texCoord.x >> texCoord.y;
			//std::cout << "v: " << texCoord.x << " " << texCoord.y << std::endl;
			objdata.vertexTexCoords.push_back(texCoord);
		}
		else if (prefix == "f")
		{
	
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			char forwardSlash;
			iss >> vertexIndex[0] >> forwardSlash >> uvIndex[0] >> forwardSlash >> normalIndex[0]
				>> vertexIndex[1] >> forwardSlash >> uvIndex[1] >> forwardSlash >> normalIndex[1]
				>> vertexIndex[2] >> forwardSlash >> uvIndex[2] >> forwardSlash >> normalIndex[2];
			objdata.vertexIndices.push_back(vertexIndex[0]);
			objdata.vertexIndices.push_back(vertexIndex[1]);
			objdata.vertexIndices.push_back(vertexIndex[2]);
			objdata.uvIndices.push_back(uvIndex[0]);
			objdata.uvIndices.push_back(uvIndex[1]);
			objdata.uvIndices.push_back(uvIndex[2]);
			objdata.normalIndices.push_back(normalIndex[0]);
			objdata.normalIndices.push_back(normalIndex[1]);
			objdata.normalIndices.push_back(normalIndex[2]);

		}
	}
	file.close();
	return objdata; //diff
}

Mesh* ObjReader::LoadObjMesh(const std::string& filename)
{
	return new Mesh(parseOBJ(filename));
}

bool ObjReader::Hello()
{
	return false;
}
