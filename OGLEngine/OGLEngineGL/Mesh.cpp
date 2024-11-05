#include "Mesh.h"
#include <GLAD/glad.h> //bruh.
#include "Shader.h"

Mesh::Mesh(float* vertices, size_t size)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //important to bind vertex array object before vertex buffer object, otherwise OpenGL doesn't know what VAO should reference. (NOTE: Martin was a little confused about this explanation in the lecture.)

	glGenBuffers(1, &VBO); //GLAD helps us out here, creating our buffer object(s) with one function instead of doing a more complex setup in raw OpenGL.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind buffer, kinda like a state machine, this is the current object, i.e. the current state.
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); //this actually puts our vertices into the buffer. GL_STATIC_DRAW: used for when data is only set once but will be used by the GPU many times, and won't be updated at any point during runtime.

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //tell the GPU how to read our array of floats (vertices[]).
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //read color part of vertices[].
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind (sorta) after we're done with the setup.
	glBindVertexArray(0); //unbind (sorta) after we're done with the setup.

}

Mesh::~Mesh() //when mesh is deleted, also delete Vertex Array and Vertex Buffer objects.
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::Draw(Shader* shader) //Draw mesh;
{
	shader->Use();
	glBindVertexArray(VAO); //only bind VAO when drawing the mesh since it already has a VBO reference already.
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0); //unbind (sorta) after we're done with the setup. This will help us in the future if we want to render a different mesh with the same Vertex Array, I guess.
}
