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

	void Update() override; //why am I doing this in the header file?

	void DrawObjectSpecificImGuiHierarchyAdjustables(std::vector<GameObject*>& vec) override;
private:

};

