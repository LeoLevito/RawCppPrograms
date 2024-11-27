#include "Mesh.h"
#include <GLAD/glad.h>
#include "Shader.h"

Mesh::Mesh(float* vertices, size_t vertexSize, unsigned int* indices, size_t indexSize)
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

Mesh::Mesh(ObjReader::ObjData objdata) //HERE IS WHERE I NEED TO FIX FOR .obj FILES TO DRAW CORRECTLY.
{
    std::vector<glm::vec3> out_vertices;
    for (size_t i = 0; i < objdata.vertexIndices.size(); i++)
    {
        unsigned int vertexIndex = objdata.vertexIndices[i];
        glm::vec3 vertex = objdata.vertices[vertexIndex - 1];
        out_vertices.push_back(vertex);
    }
    std::vector<glm::vec2> out_uvs;
    for (size_t i = 0; i < objdata.uvIndices.size(); ++i)
    {
        unsigned int uvIndex = objdata.uvIndices[i];
        glm::vec2 uv = objdata.vertexTexCoords[uvIndex - 1];
        out_uvs.push_back(uv);
    }
    std::vector<glm::vec2> out_normals;
    for (size_t i = 0; i < objdata.normalIndices.size(); ++i)
    {
        unsigned int normalIndex = objdata.normalIndices[i];
        glm::vec3 normal = objdata.vertexNormals[normalIndex - 1];
        out_normals.push_back(normal);
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW); //example, here. 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //these are probably not right, need to modify to read correctly.
    glEnableVertexAttribArray(0); //example, here. Doesn't this have to do with which of the shaders to be used? I don't remember.
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

	//Element buffer object, this will contain all our indices.
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, objdata.vertexIndices.size() * sizeof(unsigned int), objdata.vertexIndices.data(), GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0); //right? this wasn't in martin's obj contructor.
    glBindVertexArray(0);

    IndicesSize = objdata.vertexIndices.size(); //to be used in Draw();
}





//Create new constructor here for preparing .obj meshes for DRAW()





Mesh::~Mesh() //when mesh is deleted, also delete Vertex Array and Vertex Buffer objects.
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::Draw(Shader* shader) //Draw mesh;
{
	if (myTexture != NULL) 
	{
		glBindTexture(GL_TEXTURE_2D, myTexture->TextureObject); //so we have a new texture binding in Draw() because otherwise fragment shader would take the last binded texture, this allows us to use different textures for different objects (in the future).
	}

	shader->Use();
	glBindVertexArray(VAO); //only bind VAO when drawing the mesh since it already has a VBO reference already.
	


	if (IndicesSize > 0) //Draw loaded obj model. Hopefully.
	{
		glDrawElements(GL_TRIANGLES, IndicesSize, GL_UNSIGNED_INT, (void*)0);
	}
	else //Draw Cube
	{
		//ERROR, this is giving error for NVIDIA driver, so this likely doesn't conform to the OpenGL specification.
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); //note the 36, hard coded for a cube right now, we need to specify how many indices we use for it. In the future we will automate this.
	}


//insert new mesh loading code here checking for indices






	if (EBO == 0) //Draw Triangle
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	
	glBindVertexArray(0); //unbind vertex array after we're done drawing. This will help us in the future if we want to render a different mesh with the vertex array, I guess.
	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture after we're done drawing. This allows us to use different textures for different objects (in the future), since the fragment shader used for this specific mesh will use basically it's own binded texture, if I understand correctly.
}

void Mesh::ApplyTexture(Texture* texture) //Set Local myTexture so it can be used in the Draw() function.
{
	myTexture = texture;
}
