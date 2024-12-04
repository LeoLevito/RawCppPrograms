#include "GameObjectTest.h"
#include <glm.hpp>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"



//What, Pretty much all of this can be moved to the regular game object.

GameObjectTest::GameObjectTest() //constructor
{
	name = "GameObjectTest"; //maybe make this automatic to the class name?

	transformComponent = new TransformComponent;
	AddComponent(transformComponent);

	meshComponent = new MeshComponent;
	AddComponent(meshComponent);
}

GameObjectTest::~GameObjectTest() //need to delete any objects/pointers that was created with the 'new' keyword;
{
	//delete transformComponent; //does this call the destructor? Only one way to find out.
	//delete meshComponent;
}

void GameObjectTest::Update(Shader* shader)
{
	//meshComponent->position = transformComponent->position;
	//meshComponent->rotation = transformComponent->rotation;
	//meshComponent->scale = transformComponent->scale;
	GameObject::Update(shader);
}

void GameObjectTest::DrawObjectSpecificImGuiHierarchyAdjustables(std::vector<GameObject*>& vec)
{
	GameObject::DrawObjectSpecificImGuiHierarchyAdjustables(vec);
	//if (ImGui::Button("Remove Game Object"))
	//{
	//	vec.erase(std::remove(vec.begin(), vec.end(), this)); //i'm just trying to remove one thing from the vector bruh.
	//	delete this;
	//}
}
