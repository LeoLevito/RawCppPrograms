#include "GameObject.h"
#include "GLFW/glfw3.h"
#include "iostream"
//void GameObject::BeginPlay()
//{
//}
//
//void GameObject::Tick(float deltaTime)
//{
//}

GameObject::GameObject()
{
}

GameObject::~GameObject()
{

}

void GameObject::AddComponent(Component* component)
{
	componentVector.push_back(component);
}

void GameObject::Update() 
{
	for (int i = 0; i < componentVector.size(); i++)
	{
		componentVector[i]->Update();
	}
}

void GameObject::DrawObjectSpecificImGuiHierarchyAdjustables(std::vector<GameObject*>& vec)
{
}

void GameObject::LateSetComponentVariables()
{
	int componentIndex = 0;
	for (auto var : componentVector)
	{
		componentVector[componentIndex]->myCamera = myCamera;
		componentVector[componentIndex]->myProjection = myProjection;
		componentIndex++;
	}
}
