#include "ObjReader.h"
#include <iostream>
#include "Mesh.h"


//https://github.com/opengl-tutorials/ogl/blob/master/common/objloader.cpp

bool ObjReader::parseOBJ(const std::string& filename, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices; //f stands for face in .obj file. It handles indices.
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	//ObjData objdata;
	std::ifstream file(filename);
	std::string line;

	if (!file.is_open())
	{
		std::cerr << "Failed to open .obj file: " << filename << std::endl;
		return false; //diff
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
			temp_vertices.push_back(vertex);
		}
		else if (prefix == "vn")
		{
			glm::vec3 vertexNormal;
			iss >> vertexNormal.x >> vertexNormal.y >> vertexNormal.z;
			//std::cout << "vn: " << vertexNormal.x << " " << vertexNormal.y << " " << vertexNormal.z << std::endl;
			temp_normals.push_back(vertexNormal);
		}
		else if (prefix == "vt")
		{
			glm::vec2 texCoord;
			iss >> texCoord.x >> texCoord.y;
			//std::cout << "v: " << texCoord.x << " " << texCoord.y << std::endl;
			temp_uvs.push_back(texCoord);
		}
		else if (prefix == "f")
		{
	
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			char forwardSlash;

			if (!(iss >> vertexIndex[0] >> forwardSlash >> uvIndex[0] >> forwardSlash >> normalIndex[0] //this if statement may be causing considerable slow down of this operation, I should time this entire parseObj function. I should also implement mesh caching, like Emil told us to do in the MeshManager task.
				>> vertexIndex[1] >> forwardSlash >> uvIndex[1] >> forwardSlash >> normalIndex[1]
				>> vertexIndex[2] >> forwardSlash >> uvIndex[2] >> forwardSlash >> normalIndex[2])) 
			{
				std::cout << "Your .obj file can't be read by our simple parser!" << std::endl << "Please try re-exporting it from your 3D-program with other export settings!" << std::endl; //doing a check here to make it work similarly to link at the top of this page with the iss as per top answer at https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
				file.close();
				return false; //diff
			};
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	for (size_t i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i]; //gotta check if normalIndex is null

		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1]; //gotta check if normalIndex is null

		out_vertices.push_back(vertex);
		out_normals.push_back(normal);

		if (temp_uvs.size() > 0)
		{
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			out_uvs.push_back(uv);
		}
	}

	file.close();
	return true; //diff
}

//Mesh* ObjReader::LoadObjMesh(const std::string& filename, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
//{
//	return new Mesh(parseOBJ(filename,out_vertices, out_uvs, out_normals));
//}

bool ObjReader::Hello()
{
	return false;
}
