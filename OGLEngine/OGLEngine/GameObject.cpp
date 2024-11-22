#include "GameObject.h"

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

void GameObject::AddComponent(Component* component)
{
	componentVector.push_back(component);
}
