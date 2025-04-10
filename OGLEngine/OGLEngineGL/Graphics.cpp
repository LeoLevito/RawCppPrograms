#include "Graphics.h"
#include <iostream>
#include <gtc/matrix_transform.hpp>
#include <thread>
#include "ShaderManager.h"
#include "ShadowMap.h"
#include "EditorGUI.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "MeshComponent.h"

//https://www.glfw.org/docs/3.3/group__window.html#gab3fb7c3366577daef18c0023e2a8591f
//https://stackoverflow.com/questions/45880238/how-to-draw-while-resizing-glfw-window
//https://stackoverflow.com/questions/78009027/how-to-correctly-handle-window-resizing-when-using-imgui?noredirect=1
void GLFW_window_size_callback(GLFWwindow* window, int width, int height) //receives the new size of the glfw window when the main window is resized.
{
	//resizing window during simulation could result in weird behavior from physics and deltatime dependent functions.
	EditorGUI::Get().StartImGuiFrame(0.f);
	Graphics::Get().Render();
	EditorGUI::Get().RenderImGui();
	glfwSwapBuffers(window); //moved from Graphics::Render().

	std::cout << "resized window" << std::endl;
}

void GLFW_window_pos_callback(GLFWwindow* window, int xpos, int ypos) //receives the new pos of the glfw window when the main window is moved.
{
	////moving window during simulation could result in weird behavior from physics and deltatime dependent functions.
	EditorGUI::Get().StartImGuiFrame(0.f);
	Graphics::Get().Render();
	EditorGUI::Get().RenderImGui();
	glfwSwapBuffers(window); //moved from Graphics::Render().

	std::cout << "Moved window " << " " << xpos << " " << ypos << std::endl;
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

	glfwSetWindowSizeCallback(Graphics::Get().window, GLFW_window_size_callback);
	glfwSetWindowPosCallback(Graphics::Get().window, GLFW_window_pos_callback);

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

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glGenFramebuffers(1, &pickingFBO);
	glGenTextures(1, &sceneTexturePicking);

	glGenFramebuffers(1, &outlineMaskFBO);
	glGenTextures(1, &sceneTextureOutlineMask);

	glGenFramebuffers(1, &outlineActualFBO);
	glGenTextures(1, &sceneTextureOutlineActual);
}

void Graphics::Render()
{
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//RenderToSceneTexture(pickingFBO, sceneTexturePicking);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//RenderPickingPass();

	glClearColor(SceneBackgroundColor.x, SceneBackgroundColor.y, SceneBackgroundColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //used to clear various stuff, in this case we clear the color buffer bit first, every time the while loop loops, before writing a new color with the glClearColor function. I remember there being similar stuff needing to be done with Emil Ström's TinyEngine in order for us to render things and update them at runtime.


	//THIS MIGHT ALSO BE THE REASON WHY SHADOWMAPS AREN'T WORKING PROPERLY, BECAUSE THIS IS COMMENTED OUT.
	//Aside from the fact that I now need to do shader->Use() when I need it instead of doing it here in Graphics.

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
	RenderToSceneTexture(sceneFBO, sceneTexture, GL_RGB);
	glViewport(0, 0, EditorGUI::Get().sceneWindowWidth, EditorGUI::Get().sceneWindowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //To get transparency to work with ex. FragColor(vec3, 0.1f), makes it so I can render the Grid code. https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBlendFunc.xhtml
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ShaderManager::Get().depthPass = false;
	ShaderManager::Get().shader->Use();
	for (int i = 0; i < GameObjectManager::Get().gameObjects.size(); i++) //Game.gameObjectVector calls Update() on every game object implementing Update() and that Update() can call Update() in every component implementing Update().
	{
		GameObjectManager::Get().gameObjects[i]->Update();
	}
	RenderWorldGrid();
	ShaderManager::Get().shader->Use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	RenderOutlinePass();

	//we need to swap buffers
	//glfwSwapBuffers(window); //swap front and back buffers on the window. (Info: our framebuffer has two sides, the back buffer is what we add, the front buffer is what we see. Basically.). So we can see everything that was added before this function was called!
	EscapeToCloseWindow();
	//glfwPollEvents();
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
void Graphics::RenderToSceneTexture(unsigned int FBO, unsigned int texture, int rgbMode)
{

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, rgbMode, EditorGUI::Get().sceneWindowWidth, EditorGUI::Get().sceneWindowHeight, 0, rgbMode, GL_UNSIGNED_BYTE, NULL); //is this okay to do on tick?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

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

void Graphics::RenderWorldGrid()
{
	glm::mat4 trans = glm::mat4(1.0f);
	//use shader for line rendering
	ShaderManager::Get().lineShader->Use();
	ShaderManager::Get().lineShader->SetMatrix4(trans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
	ShaderManager::Get().lineShader->SetMatrix4(Camera::Get().myView, "view");
	ShaderManager::Get().lineShader->SetMatrix4(Camera::Get().projection, "projection");

	for (int i = 0; i < 5; i++)
	{
		glm::vec3 startPoint;
		glm::vec3 endPoint;
		glm::vec3 color;

		int lineLength = 500;
		int lineAmount = 500;
		if (i == 0) //X axis line
		{

			startPoint = { -lineLength,0,0 };
			endPoint = { lineLength,0,0 };
			color = { 1, 0, 0 };
			ShaderManager::Get().lineShader->SetVector3(color, "vertexColor");
			DrawLine(startPoint, endPoint);
		}
		else if (i == 1) //Z axis line
		{
			startPoint = { 0,0,-lineLength };
			endPoint = { 0,0,lineLength };
			color = { 0, 0, 1 };
			ShaderManager::Get().lineShader->SetVector3(color, "vertexColor");
			DrawLine(startPoint, endPoint);
		}
		else if (i == 2) //Y axis line
		{
			startPoint = { 0,-lineLength,0 };
			endPoint = { 0,lineLength,0 };
			color = { 0, 1, 0 };
			ShaderManager::Get().lineShader->SetVector3(color, "vertexColor");
			DrawLine(startPoint, endPoint);
		}
	}

	// positions of two triangle plane vertices
	glm::vec3 vert1 = { 1.0f,  1.0f,  0.0f };
	glm::vec3 vert2 = { 1.0f, -1.0f,  0.0f };
	glm::vec3 vert3 = { -1.0f,-1.0f,  0.0f };
	glm::vec3 vert4 = { -1.0f, 1.0f,  0.0f };

	std::vector<glm::vec3> verts = { vert1, vert2, vert3, vert4 };
	std::vector<unsigned int> indices = { 0, 1, 3, 1, 2, 3 };

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); //the indices might be wrong

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //these are probably not right, need to modify to read correctly.

	glBindVertexArray(0);

	//use shader for line rendering
	ShaderManager::Get().gridShader->Use();
	ShaderManager::Get().gridShader->SetMatrix4(trans, "model"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
	ShaderManager::Get().gridShader->SetMatrix4(Camera::Get().myView, "view");
	ShaderManager::Get().gridShader->SetMatrix4(Camera::Get().projection, "projection");
	ShaderManager::Get().gridShader->SetVector3(Camera::Get().myPosition, "camera_position");
	//ShaderManager::Get().gridShader->SetFloat(Camera::Get().nearClipLane, "near");
	//ShaderManager::Get().gridShader->SetFloat(Camera::Get().farClipLane, "far");
	ShaderManager::Get().gridShader->SetFloat(0.0f, "grid_height");

	glBindVertexArray(VAO); //only bind VAO when drawing the mesh since it already has a VBO reference already.
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//glDisable(GL_BLEND);
	ShaderManager::Get().shader->Use();
}

void Graphics::DrawLine(glm::vec3 startPoint, glm::vec3 endPoint) //color needs to be sent to the lineShader before this.
{
	glm::vec3 vert1 = startPoint;
	glm::vec3 vert2 = endPoint;

	std::vector<glm::vec3> verts = { vert1, vert2 };
	std::vector<unsigned int> indices = { 0 , 1 };

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); //the indices might be wrong

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //these are probably not right, need to modify to read correctly.

	glBindVertexArray(0);


	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0); //cast?
	glBindVertexArray(0);
}

void Graphics::DrawImgui()
{
	ImGui::DragFloat3("Scene Background Color", &SceneBackgroundColor.x, .01f, 0.0, 1.0f);
}

void Graphics::RenderPickingPass() //https://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-an-opengl-hack/
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	RenderToSceneTexture(pickingFBO, sceneTexturePicking, GL_RGB);

	ShaderManager::Get().pickingPass = true;
	ShaderManager::Get().pickingShader->Use();

	glViewport(0, 0, EditorGUI::Get().sceneWindowWidth, EditorGUI::Get().sceneWindowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < GameObjectManager::Get().gameObjects.size(); i++) //Game.gameObjectVector calls Update() on every game object implementing Update() and that Update() can call Update() in every component implementing Update().
	{
		GameObjectManager::Get().gameObjects[i]->Update();
	}
	ShaderManager::Get().pickingPass = false;


	// Wait until all the pending drawing commands are really done.
	// Ultra-mega-over slow ! 
	// There are usually a long time between glDrawElements() and
	// all the fragments completely rasterized.
	glFlush();
	glFinish();


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	ImGuiIO& io = ImGui::GetIO();
	int currentMousePosXInPickingFrameBuffer = io.MousePos.x - EditorGUI::Get().sceneWindowPosX;
	int currentMousePosYInPickingFrameBuffer = EditorGUI::Get().sceneWindowHeight - (io.MousePos.y - EditorGUI::Get().sceneWindowPosY); //Need to invert mouse Y pos since the framebuffer's Y axis is opposite of the io.MousePos.y.

	//std::cout << currentMousePosYInPickingFrameBuffer << std::endl;

	// Read the pixel at the center of the screen.
	// You can also use glfwGetMousePos().
	// Ultra-mega-over slow too, even for 1 pixel, 
	// because the framebuffer is on the GPU.
	unsigned char data[4];
	glReadPixels(currentMousePosXInPickingFrameBuffer, currentMousePosYInPickingFrameBuffer, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); //need to unbind framebuffer after glReadPixels because glReadPixels is supposed to read pixels from a frame buffer.

	// Convert the color back to an integer ID
	int pickedID =
		data[0] +
		data[1] * 256 +
		data[2] * 256 * 256;

	if (pickedID == 0x00ffffff) // Full white, must be the background !
	{
		std::cout << "background" << std::endl;
	}
	else
	{
		EditorGUI::Get().currentlySelectedGameObject = pickedID;
		std::ostringstream oss;
		oss << "mesh " << pickedID;
		std::cout << oss.str() << std::endl;
	}
}

void Graphics::RenderOutlinePass()
{
	//https://io7m.com/documents/outline-glsl/ 'Outlines Using Masking' chapter.
	//http://geoffprewett.com/blog/software/opengl-outline/ 'Post-Processing' chapter. General method part, not the code part. Helped me understand what I needed to do here. I don't do it the same way since I draw framebuffer images in the SceneWindow but the structure is similar to where I render my selected object to a mask texture and then permorm edge detection on that texture and then draw that image to the SceneWindow.

	if (!(EditorGUI::Get().currentlySelectedGameObject <= GameObjectManager::Get().gameObjects.size()))
	{
		allowOutline = false;
		return;
	}
	allowOutline = true;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //switch to GL_FILL so we don't have any wireframes rendering to the sceneTextureOutline.

	//First render outline mask to texture.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	RenderToSceneTexture(outlineMaskFBO, sceneTextureOutlineMask, GL_RGBA);
	ShaderManager::Get().outlinePass = true;
	ShaderManager::Get().outlineShader->Use();
	glViewport(0, 0, EditorGUI::Get().sceneWindowWidth, EditorGUI::Get().sceneWindowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, outlineMaskFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GameObject* owner = GameObjectManager::Get().gameObjects[EditorGUI::Get().currentlySelectedGameObject];
	for (size_t i = 0; i < owner->components.size(); i++)
	{
		if (dynamic_cast<MeshComponent*>(owner->components[i])) //checking if owner has a component of type MeshComponent. Is of-type correct word-use in this case?
		{
			dynamic_cast<MeshComponent*>(owner->components[i])->Update(); //call mesh component update, so only mesh renders to sceneTextureOutline and not anything else like debug line drawings from a Collider.
			//multiple casts, in UPDATE()? not sure how efficient this is. EDIT 5 dec 2024, apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times.
		}
	}
	ShaderManager::Get().outlinePass = false;
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //need to unbind framebuffer after glReadPixels because glReadPixels is supposed to read pixels from a frame buffer.





	//Then render actual outline to texture.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	RenderToSceneTexture(outlineActualFBO, sceneTextureOutlineActual, GL_RGBA);
	glViewport(0, 0, EditorGUI::Get().sceneWindowWidth, EditorGUI::Get().sceneWindowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, outlineActualFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderManager::Get().outlineActualShader->Use();

	
		//copied from RenderWorldGrid(), we need to have a two triangle plane so the OutlineActualShader can do its magic using the Normalized Device Coordinates positions of these vertices:
		//doing draw elements to a shader that ONLY HAS VERTEX POSITION (aPos) and not multiplying with a transform, projection or view, means that the vertices will be layed out in Normalized Device Coordinates: https://learnopengl.com/Getting-started/Hello-Triangle
		glm::vec3 vert1 = { 1.0f,  1.0f,  0.0f };
		glm::vec3 vert2 = { 1.0f, -1.0f,  0.0f };
		glm::vec3 vert3 = { -1.0f,-1.0f,  0.0f };
		glm::vec3 vert4 = { -1.0f, 1.0f,  0.0f };

		std::vector<glm::vec3> verts = { vert1, vert2, vert3, vert4 };
		std::vector<unsigned int> indices = { 0, 1, 3, 1, 2, 3 };

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); //the indices might be wrong

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //these are probably not right, need to modify to read correctly.

		glBindVertexArray(0);



	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, sceneTextureOutlineMask); //so we have a new texture binding in Draw() because otherwise fragment shader would take the last binded texture, this allows us to use different textures for different objects (in the future).
	ShaderManager::Get().outlineActualShader->SetInt(4, "gbufferMask");

	glm::vec2 viewportSize = { EditorGUI::Get().sceneWindowWidth, EditorGUI::Get().sceneWindowHeight };
	ShaderManager::Get().outlineActualShader->SetVector2(viewportSize, "viewportSize");


		glBindVertexArray(VAO); //only bind VAO when drawing the mesh since it already has a VBO reference already.
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); //doing draw elements to a shader that only has vertex position and not a transform, projection or view, means that the vertices will be layed out in Normalized Device Coordinates: https://learnopengl.com/Getting-started/Hello-Triangle
		glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); //need to unbind framebuffer after glReadPixels because glReadPixels is supposed to read pixels from a frame buffer.


	switch (EditorGUI::Get().currentPolygonMode) //revert back to the currentPolygonMode.
	{
	case GL_FILL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case GL_LINE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case GL_POINT:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	default:
		break;
	}
}
