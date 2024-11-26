#pragma once
#include <GameObject.h>
#include <TransformComponent.h>
#include <MeshComponent.h>
class GameObjectTest : public GameObject
{
public:
	GameObjectTest();
	~GameObjectTest();
	//MAKE DESTRUCTOR SO WE CAN DELETE IT.


	//void BeginPlay() override;
	//void Tick(float deltaTime) override;
	TransformComponent* transformComponent;

	MeshComponent* meshComponent;

	void Update(Shader* shader) override
	{
		meshComponent->position = transformComponent->position;
		meshComponent->rotation = transformComponent->rotation;
		meshComponent->scale = transformComponent->scale;
		GameObject::Update(shader);
	}

	void DrawObjectSpecificImGuiHierarchyAdjustables(std::vector<GameObject*>& vec) override;
private:

};

