#include "GameObjectTest.h"
#include <glm.hpp>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

GameObjectTest::GameObjectTest() //constructor
{
	std::cout << "CURRENTLY CREATING GAMEOBJECTTEST" << std::endl;
	name = "GameObjectTest";

	transformComponent = new TransformComponent;
	transformComponent->name = "Transform Component";
	AddComponent(transformComponent);

	meshComponent = new MeshComponent;
	meshComponent->name = "Mesh Component";
	AddComponent(meshComponent);
}

GameObjectTest::~GameObjectTest() //need to delete any objects/pointers that was created with the 'new' keyword;
{
	std::cout << "CURRENTLY DELETING GAMEOBJECTTEST" << std::endl;
	delete transformComponent; //does this call the destructor? Only one way to find out.
	delete meshComponent;
}

void GameObjectTest::DrawObjectSpecificImGuiHierarchyAdjustables(std::vector<GameObject*>& vec)
{
	GameObject::DrawObjectSpecificImGuiHierarchyAdjustables(vec);
	if (ImGui::Button("Remove Game Object"))
	{
		vec.erase(std::remove(vec.begin(), vec.end(), this)); //i'm just trying to remove one thing from the vector bruh.
		delete this;
	}
}

//void GameObjectTest::BeginPlay()
//{
//	//transformComponent->position = new Vector3(100, 100, 100); //Auto convert to .f/f in Vector 3 class/struct or whatever it will be, I don't like typing .f.
//
//}
//
//void GameObjectTest::Tick(float deltaTime)
//{
//
//}
