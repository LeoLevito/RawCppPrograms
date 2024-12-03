#include "GameObject.h"
#include "GLFW/glfw3.h"
#include "iostream"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
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
	for (int i = 0; i < componentVector.size(); i++)
	{
		componentVector[i]->Update(shader);
	}
}

void GameObject::DrawObjectSpecificImGuiHierarchyAdjustables(std::vector<GameObject*>& vec)
{
	static char str0[128] = "name"; //how it's done in the ImGui demo, tho it is replicated across all objects now...
	ImGui::InputText("Object name", str0, IM_ARRAYSIZE(str0)); //Yeah, I gotta change this to a dropdown or something.
	if (ImGui::Button("Change object name"))
	{
		name = str0;
	}
}

void GameObject::LateSetComponentVariables()
{
	int componentIndex = 0;
	for (auto var : componentVector)
	{
		componentVector[componentIndex]->myCamera = &Camera::Get();
		componentVector[componentIndex]->myProjection = myProjection;
		componentIndex++;
	}
}
