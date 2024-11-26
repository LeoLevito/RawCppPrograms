#include "Graphics.h"
#include <iostream>
#include <gtc/matrix_transform.hpp>

void Graphics::Initialize(int width, int height, Game* game)
{
	if (!glfwInit())	//we need to initialize glfw
	{
		std::cout << "failed to initialize glfw" << std::endl;
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "OGLEngine", NULL, NULL); //create window.
	glfwMakeContextCurrent(window); //make window current context of OpenGL calling thread.

	myWidth = width;
	myHeight = height;

	if (window == NULL)
	{
		std::cout << "failed to initialize window" << std::endl;
		return;
	}
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize GLAD" << std::endl;
		return;
	}

	myShader = new Shader; //quick fix for the 0xCCCCCCCC write/read access violation.
	myShader->Initialize("../Shaders/VertexShader.vertexs", "../Shaders/FragmentShader.fragments");
	//myTriangle = new Triangle(); //quick fix for the 0xCCCCCCCC write/read access violation. Probably need to remember to do this more.
	myCube = new Cube();
	myCamera = new Camera();
	myTexture = new Texture("../Textures/Bliss.jpg"); //I remember Martin saying that the texture will load to fragment shader without having to set the uniform variable in it.
	myCube->ApplyTexture(myTexture); 
	//myCubePositions.push_back(glm::vec3(0.0f,0.0f,0.0f));

	glEnable(GL_DEPTH_TEST); //enable depth testing, this makes it so objects in front occlude objects in back.

	for (size_t x = 0; x < 10; x++) //10x10 grid, is this how to make the 100 cubes Martin did? EDIT: It is!
	{
		for (size_t y = 0; y < 10; y++)
		{
			for (size_t z = 0; z < 10; z++)
			{
				//VirtualObject* b = new VIrtualObject(*myCube, *myTexture, *myShader);
				//myObjects->puch.back();
				myCubePositions.push_back(glm::vec3(x * 2.0f, z * 2.0f, y * 2.0f));
			}
		}
	}
	myCubePositions[0].x -= 5.5f; //test to move a single cube.
	projection = glm::perspective(glm::radians(45.0f), myWidth / myHeight, 0.1f, 100.0f);
	myGame = game;
}

void Graphics::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //used to clear various stuff, in this case we clear the color buffer bit first, every time the while loop loops, before writing a new color with the glClearColor function. I remember there being similar stuff needing to be done with Emil Ström's TinyEngine in order for us to render things and update them at runtime.

	//ExampleCube();
	for (int i = 0; i < myGame->gameObjectVector.size(); i++)
	{
		myGame->gameObjectVector[i]->Update(myShader);
	}


	//we need to swap buffers
	//glfwSwapBuffers(window); //swap front and back buffers on the window. (Info: our framebuffer has two sides, the back buffer is what we add, the front buffer is what we see. Basically.). So we can see everything that was added before this function was called!
	EscapeToCloseWindow();
	//glfwPollEvents();
}

void Graphics::Cache() //this is being done in the Mesh now. Should delete this.
{
	glGenBuffers(1, &VBO); //GLAD helps us out here, creating our buffer object(s) with one function instead of doing a more complex setup in raw OpenGL.
	glGenVertexArrays(1, &VAO);
}

bool Graphics::ShouldClose()
{
	if (glfwWindowShouldClose(window))
	{
		return true;
	}
	return false;
}

void Graphics::ExampleCube() //put this in Graphics. Currently this is orthographic.
{
    for (glm::vec3 v : myCubePositions)
    {
        glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, v); //translate first so that each object rotates independently.
        //trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

		//write to Vertex Shader
		myShader->SetMatrix4(trans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
		myShader->SetMatrix4(myCamera->myView, "view");
		myShader->SetMatrix4(projection, "projection");

        myCube->Draw(myShader);
		//myTriangle->Draw(myShader);
    }
}

void Graphics::EscapeToCloseWindow()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
