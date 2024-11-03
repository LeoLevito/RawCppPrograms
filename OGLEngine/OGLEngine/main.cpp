//#include <iostream>
//#include <glfw3.h>
//#include "Graphics.h"
#include <Graphics.h>
#include <Shader.h>
int main()
{

	Graphics graphics;
	graphics.Initialize(640, 480);

	while (!graphics.ShouldClose())
	{
		graphics.Render();
	}
	return 0;
	//	if (!glfwInit())	//we need to initialize glfw
	//	{
	//		std::cout << "failed to initialize glfw" << std::endl;
	//		return -1;
	//	}
	//
	//	GLFWwindow* window;
	//	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL); //create window.
	//
	//	glfwMakeContextCurrent(window); //make window current context of OpenGL calling thread.
	//
	//	while (true)
	//	{
	//		glClear(GL_COLOR_BUFFER_BIT); //used to clear various stuff, in this case we clear the color buffer bit first, every time the while loop loops, before writing a new color with the glClearColor function. I remember there being similar stuff needing to be done with Emil Ström's TinyEngine in order for us to render things and update them at runtime.
	//
	//		glClearColor(0.4, 0.3, 0.6, 1); //set color, I guess.
	//
	//		//draw old OGL triangle.
	//		glBegin(GL_TRIANGLES);
	//		glVertex2f(0.5f, 0.0f);
	//		glVertex2f(0.0f, 0.5f);
	//		glVertex2f(-0.5f, 0.0f);
	//		glEnd();
	//
	//		//we need to swap buffers
	//		glfwSwapBuffers(window); //swap front and back buffers on the window. (Info: our framebuffer has two sides, the back buffer is what we add, the front buffer is what we see. Basically.). So we can see everything that was added before this function was called!
	//	
	//	
	//	}
	//
	return 0;
}