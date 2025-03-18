#include "Graphics.h"
#include <iostream>
#include <gtc/matrix_transform.hpp>
#include <thread>
#include "ShaderManager.h"
#include "ShadowMap.h"
#include "EditorGUI.h"

//https://www.glfw.org/docs/3.3/group__window.html#gab3fb7c3366577daef18c0023e2a8591f
//https://stackoverflow.com/questions/45880238/how-to-draw-while-resizing-glfw-window
//https://stackoverflow.com/questions/78009027/how-to-correctly-handle-window-resizing-when-using-imgui?noredirect=1
void GLFW_window_size_callback(GLFWwindow* window, int width, int height) //receives the new size of the glfw frame buffer when the main window is resized.
{
	//resizing window during simulation could result in weird behavior from physics and deltatime dependent functions.
	EditorGUI::Get().StartImGuiFrame(0.f);
	Graphics::Get().Render();
	EditorGUI::Get().RenderImGui();
	glfwSwapBuffers(window); //moved from Graphics::Render().
	std::cout << "resized window" << std::endl;
}

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

Graphics& Graphics::Get()
{
	static Graphics instance;
	return instance;
	// TODO: insert return statement here
}

void Graphics::Initialize(int width, int height)
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

	glfwSetFramebufferSizeCallback(Graphics::Get().window, GLFW_window_size_callback);

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

	glEnable(GL_DEPTH_TEST); //enable depth testing, this makes it so objects in front occlude objects in back.

	glGenFramebuffers(1, &sceneFBO); //only wanna do this once, otherwise memory would be allocated each tick.
	glGenTextures(1, &sceneTexture); //only wanna do this once, otherwise memory would be allocated each tick.
	glGenRenderbuffers(1, &rbo);
}

void Graphics::Render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //used to clear various stuff, in this case we clear the color buffer bit first, every time the while loop loops, before writing a new color with the glClearColor function. I remember there being similar stuff needing to be done with Emil Ström's TinyEngine in order for us to render things and update them at runtime.



//1. depth texture scene from light's perspective.
	////glCullFace(GL_FRONT);
	//ShaderManager::Get().depthPass = true;
	//ShaderManager::Get().depthShader->Use();

	//glViewport(0, 0, ShaderManager::Get().shadowMap->SHADOW_WIDTH, ShaderManager::Get().shadowMap->SHADOW_HEIGHT);
	//glBindFramebuffer(GL_FRAMEBUFFER, ShaderManager::Get().shadowMap->depthMapFBO);
	//glClear(GL_DEPTH_BUFFER_BIT);

	//for (int i = 0; i < GameObjectManager::Get().gameObjects.size(); i++) //Game.gameObjectVector calls Update() on every game object implementing Update() and that Update() can call Update() in every component implementing Update().
	//{
	//	GameObjectManager::Get().gameObjects[i]->Update();
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	////glCullFace(GL_BACK);




	ShaderManager::Get().depthPass = true;
	ShaderManager::Get().depthCubeMapShader->Use();

	glViewport(0, 0, ShaderManager::Get().shadowCubeMap->SHADOW_WIDTH, ShaderManager::Get().shadowCubeMap->SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, ShaderManager::Get().shadowCubeMap->depthCubeMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < GameObjectManager::Get().gameObjects.size(); i++) //Game.gameObjectVector calls Update() on every game object implementing Update() and that Update() can call Update() in every component implementing Update().
	{
		GameObjectManager::Get().gameObjects[i]->Update();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// do I even need to do if(ShaderManager::Get().depthPass == false)? Can you set values for a shader without the shader currently being in 'Use()'? Maybe not?




//2. render scene like usual, now using the generated depth texture/shadowmap.
	RenderToSceneTexture();
	glViewport(0, 0, EditorGUI::Get().sceneWindowWidth, EditorGUI::Get().sceneWindowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ShaderManager::Get().depthPass = false;
	ShaderManager::Get().shader->Use();

	for (int i = 0; i < GameObjectManager::Get().gameObjects.size(); i++) //Game.gameObjectVector calls Update() on every game object implementing Update() and that Update() can call Update() in every component implementing Update().
	{
		GameObjectManager::Get().gameObjects[i]->Update();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
		
		ShaderManager::Get().shader->SetMatrix4(trans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
		ShaderManager::Get().shader->SetMatrix4(Camera::Get().myView, "view");
		ShaderManager::Get().shader->SetMatrix4(Camera::Get().projection, "projection");

        myCube->Draw();
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

//https://learnopengl.com/Advanced-OpenGL/Framebuffers
void Graphics::RenderToSceneTexture()
{
	glBindTexture(GL_TEXTURE_2D, sceneTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, EditorGUI::Get().sceneWindowWidth, EditorGUI::Get().sceneWindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); //is this okay to do on tick?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTexture, 0);

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, EditorGUI::Get().sceneWindowWidth, EditorGUI::Get().sceneWindowHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
