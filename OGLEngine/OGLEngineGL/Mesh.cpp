#include "Mesh.h"
#include <GLAD/glad.h>
#include "Shader.h"

Mesh::Mesh(float* vertices, size_t vertexSize, unsigned int* indices, size_t indexSize)
{




//set IndicesSize to 0 here, in this constructor we want to be 0.




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





//Create new constructor here for preparing .obj meshes for DRAW()





Mesh::~Mesh() //when mesh is deleted, also delete Vertex Array and Vertex Buffer objects.
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);




//insert EBO buffer deletion here




}

void Mesh::Draw(Shader* shader) //Draw mesh;
{
	if (myTexture != NULL) 
	{
		glBindTexture(GL_TEXTURE_2D, myTexture->TextureObject); //so we have a new texture binding in Draw() because otherwise fragment shader would take the last binded texture, this allows us to use different textures for different objects (in the future).
	}

	shader->Use();
	glBindVertexArray(VAO); //only bind VAO when drawing the mesh since it already has a VBO reference already.
	





//insert new mesh loading code here checking for indices






	if (EBO == 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	else
	{
		//ERROR, this is giving error for NVIDIA driver, so this likely doesn't conform to the OpenGL specification.
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); //note the 36, hard coded for a cube right now, we need to specify how many indices we use for it. In the future we will automate this.
	}
	glBindVertexArray(0); //unbind vertex array after we're done drawing. This will help us in the future if we want to render a different mesh with the vertex array, I guess.
	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture after we're done drawing. This allows us to use different textures for different objects (in the future), since the fragment shader used for this specific mesh will use basically it's own binded texture, if I understand correctly.
}

void Mesh::ApplyTexture(Texture* texture) //Set Local myTexture so it can be used in the Draw() function.
{
	myTexture = texture;
}
