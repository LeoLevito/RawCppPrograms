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

void GameObject::Update(Shader* shader) 
{
	//for (int i = 0; i < componentVector.size(); i++)
	//{
	//	componentVector[i]->Update();
	//}
	componentVector[1]->Update(shader);
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
