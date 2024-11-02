#include "Graphics.h"
#include <iostream>
#include <GLAD/glad.h> //bruh.

void Graphics::Initialize(int width, int height)
{
	if (!glfwInit())	//we need to initialize glfw
	{
		std::cout << "failed to initialize glfw" << std::endl;
		return;
	}

	//new stuff.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL); //create window.
	glfwMakeContextCurrent(window); //make window current context of OpenGL calling thread.

	//new stuff
	if (window == NULL)
	{
		std::cout << "failed to initialize window" << std::endl;
		return;
	}
	//new stuff
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize GLAD" << std::endl;
		return;
	}
}

void Graphics::Render()
{
	glClear(GL_COLOR_BUFFER_BIT); //used to clear various stuff, in this case we clear the color buffer bit first, every time the while loop loops, before writing a new color with the glClearColor function. I remember there being similar stuff needing to be done with Emil Ström's TinyEngine in order for us to render things and update them at runtime.

	//let's do triangle
	float vertices[] =
	{
		//normalized device coordinates, think x and y axis of your screen/window, from -1 to 1 on either axis.
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	unsigned int VBO; //vertex buffer object, OpenGL objects are unsigned ints. We can put a bunch of vertices in this object and send it to the GPU.
	glGenBuffers(1, &VBO); //GLAD helps us out here, creating our buffer object(s) with one function instead of doing a more complex setup in raw OpenGL.

	//example of multiple buffer objects:
			//unsigned int VBOs[5]; 
			//glGenBuffers(5, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind buffer, kinda like a state machine, this is the current object, i.e. the current state.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //this actually puts our vertices into the buffer. GL_STATIC_DRAW: used for when data is only set once but will be used by the GPU many times, and won't be updated at any point during runtime.
	//----------->> 3rd lecture video (still 2nd lecture) at 16:45 is next step. <<-----------

	//we need to swap buffers
	glfwSwapBuffers(window); //swap front and back buffers on the window. (Info: our framebuffer has two sides, the back buffer is what we add, the front buffer is what we see. Basically.). So we can see everything that was added before this function was called!

	//new stuff
	glfwPollEvents();
}


//new stuff
bool Graphics::ShouldClose()
{
	if (glfwWindowShouldClose(window))
	{
		return true;
	}
	return false;
}
