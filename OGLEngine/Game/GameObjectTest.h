#pragma once
#include <GameObject.h>
#include <TransformComponent.h>
class GameObjectTest : public GameObject
{
public:
	GameObjectTest();
	//void BeginPlay() override;
	//void Tick(float deltaTime) override;
private:
	TransformComponent* transformComponent;
};

