#include "GameObjectTest.h"
#include <glm.hpp>

GameObjectTest::GameObjectTest() //constructor
{
	transformComponent = new TransformComponent;
	transformComponent->position = glm::vec3(1, 1, 1);
	//transformComponent = AddComponent(TransformComponent);
	//meshComponent = AddComponent(MeshComponent);

	//should probably have these position, rotation and scale have default values in the component class.
	//transformComponent->position = new Vector3(0, 0, 0); //could probably use GLM::vec3 function here.
	//transformComponent->rotation = new Vector3(0,0,0) //maybe vector3 isn't the best for rotations. //could probably use GLM functions.
	//transformComponent->scale = new Vector3(1,1,1) 
	
	//meshComponent->mesh = targetMesh;
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
