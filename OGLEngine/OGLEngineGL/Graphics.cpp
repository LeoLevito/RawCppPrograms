#include "Graphics.h"
#include <iostream>
//#include <GLAD/glad.h> //bruh.

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
	Cache();
	myShader->Initialize("../Shaders/VertexShader.vertexs","../Shaders/FragmentShader.fragments");
}

void Graphics::Render()
{
	glClear(GL_COLOR_BUFFER_BIT); //used to clear various stuff, in this case we clear the color buffer bit first, every time the while loop loops, before writing a new color with the glClearColor function. I remember there being similar stuff needing to be done with Emil Ström's TinyEngine in order for us to render things and update them at runtime.

	myTriangleMesh->Draw(myShader);

	//we need to swap buffers
	glfwSwapBuffers(window); //swap front and back buffers on the window. (Info: our framebuffer has two sides, the back buffer is what we add, the front buffer is what we see. Basically.). So we can see everything that was added before this function was called!

	//new stuff
	glfwPollEvents();
}

void Graphics::Cache()
{
	glGenBuffers(1, &VBO); //GLAD helps us out here, creating our buffer object(s) with one function instead of doing a more complex setup in raw OpenGL.
	glGenVertexArrays(1, &VAO);
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
