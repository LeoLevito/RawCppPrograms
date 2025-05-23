#include <Graphics.h>
#include <Engine.h>
#include <EditorGUI.h>
#include <GameObjectManager.h>
#include <gtc/matrix_transform.hpp>
#include <Thread.h>
#include <thread>
#include <mutex>
#include <iostream>
#include "MeshManager.h"
#include "ShaderManager.h"
#include "MemoryCheckManager.h"
#include "CollisionManager.h"
#include "TextureManager.h"

int main()
{
	Graphics::Get().Initialize(1280, 720);

	Engine::Get().Initialize(Graphics::Get().window, &Camera::Get());

	EditorGUI::Get().Initialize(Graphics::Get().window, &Graphics::Get(), Camera::Get());

	std::thread MeshManagerThread(&MeshManager::Process, &MeshManager::Get()); //make a wrapper function in Thread class that does std::thread t1(func).
	std::thread GameObjectManagerThread(&GameObjectManager::Process, &GameObjectManager::Get()); //make a wrapper function in Thread class that does std::thread t1(func).
	std::thread MemoryCheckManagerThread(&MemoryCheckManager::Process, &MemoryCheckManager::Get());
	std::thread CollisionManagerThread(&CollisionManager::Process, &CollisionManager::Get());

	float lastTime = 0;
	float currentTime = 0;
	float deltaTime = 0;

	glfwSwapInterval(1); //set max framerate, think (1/50) * 50 for 1 fps at 50hz, this doesn't function like a VSYNC toggle it seems to my eyes. Edit: well, when a big mesh is rendering it does seem to lock the framerate perfectly.

	ShaderManager::Get().Initialize(); //initialize shadermanager with shadowmap.
	TextureManager::Get(); //initialize TextureManager.
	while (!Graphics::Get().ShouldClose())
	{
		currentTime = glfwGetTime(); //May or may not be super accurate at the moment.
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glfwPollEvents(); //moved from Graphics::Render().


		Engine::Get().Update(Graphics::Get().window, deltaTime);
		Graphics::Get().Render();

		EditorGUI::Get().StartImGuiFrame(deltaTime);
		EditorGUI::Get().RenderImGui();

		glfwSwapBuffers(Graphics::Get().window); //moved from Graphics::Render().
	}

	//terminate thread.
	MeshManager::Get().shouldRun = false;
	MeshManagerThread.join();
	MeshManagerThread.~thread();

	//terminate thread.
	GameObjectManager::Get().shouldRun = false;
	GameObjectManagerThread.join();
	GameObjectManagerThread.~thread();

	//terminate thread.
	MemoryCheckManager::Get().shouldRun = false;
	MemoryCheckManagerThread.join();
	MemoryCheckManagerThread.~thread();

	//terminate thread.
	CollisionManager::Get().shouldRun = false;
	CollisionManagerThread.join();
	CollisionManagerThread.~thread();

	EditorGUI::Get().CloseImGui();
	return 0;
}


