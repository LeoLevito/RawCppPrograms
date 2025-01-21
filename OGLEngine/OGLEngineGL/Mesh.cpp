#include "Mesh.h"
#include <GLAD/glad.h>
#include "Shader.h"
#include <iostream>
#include <thread>
#include "ShaderManager.h"

Mesh::Mesh(float* vertices, size_t vertexSize, unsigned int* indices, size_t indexSize) //Old cube constructor, right?
{
//set IndicesSize to 0 here, in this constructor we want to be 0.
	IndicesSize = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //important to bind vertex array object before vertex buffer object, otherwise OpenGL doesn't know what VAO should reference. (NOTE: Martin was a little confused about this explanation in the lecture.)

	glGenBuffers(1, &VBO); //GLAD helps us out here, creating our buffer object(s) with one function instead of doing a more complex setup in raw OpenGL.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind buffer, kinda like a state machine, this is the current object, i.e. the current state.
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW); //this actually puts our vertices into the buffer. GL_STATIC_DRAW: used for when data is only set once but will be used by the GPU many times, and won't be updated at any point during runtime.

	EBO = 0; //Element buffer object, this will contain all our indices.
	if (indices && indexSize > 0) //Rendering a single triangle doesn't need an Element buffer object, so we have a check here if indices are used or not before binding the EBO.
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //tell the GPU how to read our array of floats (vertices[]).
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //read color part of vertices[].
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind (sorta) after we're done with the setup.
	glBindVertexArray(0); //unbind (sorta) after we're done with the setup.

}

Mesh::Mesh(ObjReader* objreader, const std::string& filename) //HERE IS WHERE I NEED TO FIX FOR .obj FILES TO DRAW CORRECTLY.
{
	//https://github.com/opengl-tutorials/ogl/blob/master/tutorial07_model_loading/tutorial07.cpp
	//https://github.com/opengl-tutorials/ogl/blob/master/tutorial09_vbo_indexing/tutorial09.cpp

	meshLoadedCorrectly = false;

	bool res = objreader->parseOBJ(filename, indices, indexed_vertices, indexed_uvs, indexed_normals); //reason for big meshes not rendering properly: https://stackoverflow.com/a/26426240, now fixed.

	if (!res) //can just call parse obj in an if statement instead of doing this after calling it.
	{
		meshLoadedCorrectly = false;
		return;
	}
	else
	{
		meshLoadedCorrectly = true;
	}


	//5 dec 2024, Emil suggested just saving any variables used for rendering the mesh, like vertices and faces, into a file.
	//so maybe the fast way of getting this working is to write all vectors here into a file right here.

//IMPLEMENT TRIANGLULATION FOR .OBJ FILES BUILT OUT OF QUADS.




}

Mesh::~Mesh() //when mesh is deleted, also delete Vertex Array and Vertex Buffer objects.
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &UVBO);
	glDeleteBuffers(1, &NBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::Draw() //Draw mesh;
{
	if (diffuseMap != NULL) 
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap->TextureObject); //so we have a new texture binding in Draw() because otherwise fragment shader would take the last binded texture, this allows us to use different textures for different objects (in the future).
	}

	if (specularMap != NULL)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap->TextureObject); //so we have a new texture binding in Draw() because otherwise fragment shader would take the last binded texture, this allows us to use different textures for different objects (in the future).
	}

	//if (myShadowMap != NULL)
	//{
	//	glActiveTexture(GL_TEXTURE2);
	//	glBindTexture(GL_TEXTURE_2D, myShadowMap->depthMap);
	//}



	//ShaderManager::Get().shader->Use();




	glBindVertexArray(VAO); //only bind VAO when drawing the mesh since it already has a VBO reference already.

	if (IndicesSize > 0) //Draw loaded obj model. Hopefully.
	{
		//glDrawArrays(GL_TRIANGLES, 0, 50);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
	}
	else //Draw Cube
	{
		//ERROR, this is giving error for NVIDIA driver, so this likely doesn't conform to the OpenGL specification.
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); //note the 36, hard coded for a cube right now, we need to specify how many indices we use for it. In the future we will automate this.
	}

	if (EBO == 0) //Draw Triangle
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	
	glBindVertexArray(0); //unbind vertex array after we're done drawing. This will help us in the future if we want to render a different mesh with the vertex array, I guess.
	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture after we're done drawing. This allows us to use different textures for different objects (in the future), since the fragment shader used for this specific mesh will use basically it's own binded texture, if I understand correctly.
}

void Mesh::ApplyDiffuseMap(Texture* texture) //Set Local myTexture so it can be used in the Draw() function.
{
	diffuseMap = texture;
}

void Mesh::ApplySpecularMap(Texture* texture)
{
	specularMap = texture;
}

//void Mesh::ApplyShadowMap(ShadowMap* shadowMap)
//{
//	myShadowMap = shadowMap;
//}

void Mesh::bufferMesh()
{
	//open gl calls should happen in the same context, so calling this mesh constructor in a separate thread messes things up.
	//need to maybe separate this into a new function and call it on the Graphics thread context instead.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//https://github.com/opengl-tutorials/ogl/blob/master/tutorial09_vbo_indexing/tutorial09.cpp
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW); 

	glGenBuffers(1, &UVBO);
	glBindBuffer(GL_ARRAY_BUFFER, UVBO);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW); 

	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW); 

	//Element buffer object, this will contain all our indices.
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//https://github.com/opengl-tutorials/ogl/blob/master/tutorial09_vbo_indexing/tutorial09.cpp
	//vertex positions
	glEnableVertexAttribArray(0); //example, here. Doesn't this have to do with which of the shaders to be used? I don't remember.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //these are probably not right, need to modify to read correctly.
	//vertex texture coords
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//vertex normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glBindBuffer(GL_ARRAY_BUFFER, 0); //right? this wasn't in martin's obj contructor.
	glBindVertexArray(0);

	IndicesSize = indices.size(); //to be used in Draw();
}
