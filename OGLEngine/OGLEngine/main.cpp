#include <Graphics.h>
#include <Engine.h>
#include <EditorGUI.h>
#include <GameObjectManager.h>
#include <gtc/matrix_transform.hpp>
#include <Thread.h>
#include <thread>
#include <mutex>
#include <iostream>

//float myWidth = 1280;
//float myHeight = 720;
//static glm::mat4* GLOBALprojection = &glm::perspective(glm::radians(45.0f), myWidth / myHeight, 0.1f, 100.0f);

int mutexTest = 0;
std::mutex mutexTest_mutex;
void voidfunc(int input) {
	//std::lock_guard<std::mutex> myGuard(mutexTest_mutex);
	int rama = 0;
	for (int i = 0; i < 900000001; i++)
	{
		std::lock_guard<std::mutex> myGuard(mutexTest_mutex); //where to put mutex locks for proper thread writing to same variable? Inside for loop, outside? Before or after?
		std::cout << "Thread: " << input << " has control." << std::endl;
		rama = input + i;
		//mutexTest = input + i; //so multiple threads trying to access this might be causing slowdown when mutex is locked, which is a good thing. But multithreading is SIGNIFICANTLY FASTER when using a local variable that's created by each thread. So maybe try to minimize two threads accessing the same data, and if it's needed use a mutex.
	}
	//mutexTest = input;
	//std::lock_guard<std::mutex> myGuard(mutexTest_mutex); //having the mutex lock here before the print results in correct std::endl usage.
	//std::cout << rama << std::endl; //so mutexTest not safe to access right now, No it should be, we should be able to do stuff with this.
	std::cout << mutexTest << std::endl; //so mutexTest not safe to access right now, No it should be, we should be able to do stuff with this.
}
int main()
{
	//std::thread t1(voidfunc, 1);
	//std::thread t2(voidfunc, 2);
	//std::thread t3(voidfunc, 3);
	//std::thread t4(voidfunc, 4);
	//std::thread t5(voidfunc, 5);
	//std::thread t6(voidfunc, 6);
	//std::thread t7(voidfunc, 7);
	//std::thread t8(voidfunc, 8);
	//std::thread t9(voidfunc, 9);
	//std::thread t10(voidfunc, 10);
	//std::thread t11(voidfunc, 11);
	//std::thread t12(voidfunc, 12);
	//std::thread t13(voidfunc, 13);
	//std::thread t14(voidfunc, 14);
	//std::thread t15(voidfunc, 15);
	//std::thread t16(voidfunc, 16);
	//std::thread t17(voidfunc, 17);
	//std::thread t18(voidfunc, 18);
	//std::thread t19(voidfunc, 19);
	//std::thread t20(voidfunc, 20);
	//std::cout << "Starting multithreading" << std::endl;
	//std::cout << mutexTest << std::endl;
	//t1.join();
	//t2.join();
	//t3.join();
	//t4.join();
	//t5.join();
	//t6.join();
	//t7.join();
	//t8.join();
	//t9.join();
	//t10.join();
	//t11.join();
	//t12.join();
	//t13.join();
	//t14.join();
	//t15.join();
	//t16.join();
	//t17.join();
	//t18.join();
	//t19.join();
	//t20.join();
	//std::cout << "Starting singlethreading" << std::endl;
	////std::cout << mutexTest << std::endl; //mutexTest is now safe to access since the threads are joined.
	//voidfunc(1);
	//voidfunc(2);
	//voidfunc(3);
	//voidfunc(4);
	//voidfunc(5);
	//voidfunc(6);
	//voidfunc(7);
	//voidfunc(8);
	//voidfunc(9);
	//voidfunc(10);
	//voidfunc(11);
	//voidfunc(12);
	//voidfunc(13);
	//voidfunc(14);
	//voidfunc(15);
	//voidfunc(16);
	//voidfunc(17);
	//voidfunc(18);
	//voidfunc(19);
	//voidfunc(20);
	//std::cout << mutexTest << std::endl; //mutexTest is now safe to access since the threads are joined.
	////woah I don't really know what's happening right now, is it using multiple threads at different times and that's why the int is updated out of order every time?
	//

	GameObjectManager* gameObjectManager = new GameObjectManager;
	Camera::Allocate();

	
	Graphics* graphics = new Graphics;
	graphics->Initialize(1280, 720, gameObjectManager);

	Engine* engine = new Engine;
	engine->Initialize(graphics->window, &Camera::Get());

	EditorGUI* editorGUI = new EditorGUI;
	editorGUI->Initialize(graphics->window, graphics, gameObjectManager, Camera::Get(), *graphics->myShader);

	float lastTime = 0;
	float currentTime = 0;
	float deltaTime = 0;

	glfwSwapInterval(1); //set max framerate, think (1/50) * 50 for 1 fps at 50hz, this doesn't function like a VSYNC toggle it seems to my eyes.

	while (!graphics->ShouldClose())
	{
		currentTime = glfwGetTime(); //May or may not be super accurate at the moment.
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glfwPollEvents(); //moved from Graphics::Render().
		editorGUI->StartImGuiFrame(deltaTime);

		engine->Update(graphics->window,deltaTime);
		graphics->Render();

		//std::thread t1(&EditorGUI::HierarchyWindow, editorGUI, std::ref(Camera::Get()), std::ref(Camera::Get().projection), std::ref(*graphics->myShader));
		//t1.join();
		editorGUI->HierarchyWindow(Camera::Get(), Camera::Get().projection, *graphics->myShader);

		/*std::thread t2(&EditorGUI::RenderImGui, editorGUI, std::ref(Camera::Get().projection));
		t2.join();*/
		editorGUI->RenderImGui(Camera::Get().projection);
		glfwSwapBuffers(graphics->window); //moved from Graphics::Render().
	}
	

	editorGUI->CloseImGui();
	return 0;
}


