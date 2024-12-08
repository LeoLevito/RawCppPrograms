#include "ObjReader.h"
#include <iostream>
#include "Mesh.h"



//https://github.com/opengl-tutorials/ogl/blob/master/common/objloader.cpp

ObjReader::ObjReader()
{
	myVBOindexer = new vboindexer;
}

bool ObjReader::parseOBJ(const std::string& filename, std::vector<unsigned short>& indices, std::vector<glm::vec3>& indexed_vertices, std::vector<glm::vec2>& indexed_uvs, std::vector<glm::vec3>& indexed_normals)
{
	//moved vectors from mesh.cpp to here for vboindex call further down.
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::string filename2 = filename;
	filename2.append(".test");

	std::ifstream file2(filename2);
	if (file2.is_open())
	{
		file2.close(); //is there no better way of checking whether a file exists or not?
		std::cout << "binary file was found, starting deserialization of it." << std::endl;
		Deserialization(filename2);
		indices = myIndices;
		indexed_vertices = myIndexed_vertices;
		indexed_uvs = myIndexed_uvs;
		indexed_normals = myIndexed_normals;
	}
	else {
		std::string filename3 = filename;
		filename3.append(".obj");


		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices; //f stands for face in .obj file. It handles indices.
		std::vector<glm::vec3> temp_vertices;
		std::vector<glm::vec2> temp_uvs;
		std::vector<glm::vec3> temp_normals;

		std::ifstream file(filename3);
		std::string line;

		if (!file.is_open())
		{
			std::cerr << "Failed to open .obj file: " << filename3 << std::endl;
			return false;
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
				temp_vertices.push_back(vertex);
			}
			else if (prefix == "vn")
			{
				glm::vec3 vertexNormal;
				iss >> vertexNormal.x >> vertexNormal.y >> vertexNormal.z;
				temp_normals.push_back(vertexNormal);
			}
			else if (prefix == "vt")
			{
				glm::vec2 texCoord;
				iss >> texCoord.x >> texCoord.y;
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
					return false;
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

			//IMPLEMENT TRIANGLULATION FOR .OBJ FILES BUILT OUT OF QUADS.
			//check 5 dec 2024 second recording, Zongying had a solution for checking and rendering quads!
		}
		for (size_t i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i]; //gotta check if normalIndex is null

			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			glm::vec3 normal = temp_normals[normalIndex - 1]; //gotta check if normalIndex is null

			vertices.push_back(vertex);
			uvs.push_back(uv);
			normals.push_back(normal);
			
			//out_vertices.push_back(vertex);
			//out_uvs.push_back(uv);
			//out_normals.push_back(normal);
		}

		file.close();



		myVBOindexer->indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
		
		myIndices = indices;
		myIndexed_vertices = indexed_vertices;
		myIndexed_uvs = indexed_uvs;
		myIndexed_normals = indexed_normals;


		Serialization(filename2);
	}



	//moved these from mesh for vboindex call below, though maybe these should still be in mesh since they are used there...





	//might want to move the vboindexer call in Mesh into here.


	//Okay so the basic principle of today's first task (5 dec 2024) is to parse a .obj file once, 
	//write the results to a file, and then read the file when we want to load a mesh. 
	//Doesn't Unity have a meta file system that might be doing a similar thing with meshes? 
	//Or maybe meta-files are used for something else.
	//Anyways, I don't think I will do it completely like Emil has it in his powerpoint, 
	//since the code for WriteTo and ReadFrom had no examples,				//EDIT: Emil said WriteTo and ReadFrom is just an encapsulation of the .write and .read functions. So we could just do .write and .read in the for loops.
	//or maybe those were the examples before the header showcase?
	//His code must be generalized to some extent.
	//I know what the basic concept is, give me time and I'll figure out how to write to a new file, write the results of the parse, and possibly even the vboindexer, and then read from it when I want to load a mesh. Could even connect ImGui to read those files names.
	//The point is also to write and read from a binary file, so instead of using << / >> to write and read from a text file, we use .write and .read for binary files.






	//here: write to file the results of the parse after file has been closed.

	return true;
}

bool ObjReader::ReadFrom(std::iostream& file)
{
	return false;
}

bool ObjReader::WriteTo(std::iostream& file) const
{

	return false;
}

void ObjReader::Serialization(const std::string& filename)
{
	std::fstream file;

	file.open(filename.c_str(), std::ios_base::out | std::ios_base::binary); //binary files require the use of 
	////write meta-data first, if you want
	if (file.is_open())
	{
		int a = myIndexed_vertices.size();
		int b = myIndexed_uvs.size();
		int c = myIndexed_normals.size();
		int d = myIndices.size();
		//write length of vectors, https://stackoverflow.com/a/31213593
		file.write(reinterpret_cast<char*>(&a), sizeof(a));
		file.write(reinterpret_cast<char*>(&b), sizeof(b));
		file.write(reinterpret_cast<char*>(&c), sizeof(c));
		file.write(reinterpret_cast<char*>(&d), sizeof(d));

		//write contents of vectors, https://stackoverflow.com/a/31213593
		file.write(reinterpret_cast<char*>(&myIndexed_vertices[0]), sizeof(glm::vec3)* myIndexed_vertices.size());
		file.write(reinterpret_cast<char*>(&myIndexed_uvs[0]), sizeof(glm::vec2) * myIndexed_uvs.size());
		file.write(reinterpret_cast<char*>(&myIndexed_normals[0]), sizeof(glm::vec3) * myIndexed_normals.size());
		file.write(reinterpret_cast<char*>(&myIndices[0]), sizeof(unsigned short) * myIndices.size());
	}
	file.close();
	//	for (int i = 0; i < Vertices; i++) //might be possible to use Vertices.data() instead of for looping through every element. Google how to write and read a c++ vector from a binary file.
	//	{
	//		vertices[i].WriteTo(file);
	//	}
	//	for (int i = 0; i < UVs; i++)
	//	{
	//		uvs[i].WriteTo(file);
	//	}
	//	for (int i = 0; i < Normals; i++)
	//	{
	//		normals[i].WriteTo(file);
	//	}
	//	for (int i = 0; i < Faces; i++)
	//	{
	//		faces[i].WriteTo(file);
	//	}
	//}
	//file.close();
}

void ObjReader::Deserialization(const std::string& filename)
{
	std::fstream file;
	file.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
	////read meta-data if you wrote that.
	if (file.is_open())
	{
		
		int vertice;
		int uv;
		int normal;
		int index;

		//read sizes of vectors, https://stackoverflow.com/a/31213593
		file.read(reinterpret_cast<char*>(&vertice), sizeof(vertice)); //I see the issue now, we don't know the size of the vector when we want to read it, and since we input the size of an empty vector right now, it's gonna return nothing as well. so maybe we need to write the size to the file for us to retrieve when reading it.
		file.read(reinterpret_cast<char*>(&uv), sizeof(uv));
		file.read(reinterpret_cast<char*>(&normal), sizeof(normal));
		file.read(reinterpret_cast<char*>(&index), sizeof(index));

		myIndexed_vertices.resize(vertice);
		myIndexed_uvs.resize(uv);
		myIndexed_normals.resize(normal);
		myIndices.resize(index);

		//read contents of vectors, https://stackoverflow.com/a/31213593
		file.read(reinterpret_cast<char*>(&myIndexed_vertices[0]), sizeof(glm::vec3) * myIndexed_vertices.size());
		file.read(reinterpret_cast<char*>(&myIndexed_uvs[0]), sizeof(glm::vec2) * myIndexed_uvs.size());
		file.read(reinterpret_cast<char*>(&myIndexed_normals[0]), sizeof(glm::vec3)* myIndexed_normals.size());
		file.read(reinterpret_cast<char*>(&myIndices[0]), sizeof(unsigned short) * myIndices.size());

		//allocate arrays for all data to be read, based on the numbers above.
		//AllocateArrays();
	}
	file.close();
	//	//read all the data
	//	for (int i = 0; i < Vertices; i++)
	//	{
	//		vertices[i].ReadFrom(file);
	//	}
	//	for (int i = 0; i < UVs; i++)
	//	{
	//		uvs[i].ReadFrom(file);
	//	}
	//	for (int i = 0; i < Normals; i++)
	//	{
	//		normals[i].ReadFrom(file);
	//	}
	//	for (int i = 0; i < Faces; i++)
	//	{
	//		faces[i].ReadFrom(file);
	//	}
	//}
	//
	//file.close();
}
