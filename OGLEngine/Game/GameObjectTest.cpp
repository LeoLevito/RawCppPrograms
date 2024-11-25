#include "GameObjectTest.h"
#include <glm.hpp>
#include <iostream>

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

	//transformComponent = AddComponent(TransformComponent);
	//meshComponent = AddComponent(MeshComponent);

	//should probably have these position, rotation and scale have default values in the component class.
	//transformComponent->position = new Vector3(0, 0, 0); //could probably use GLM::vec3 function here.
	//transformComponent->rotation = new Vector3(0,0,0) //maybe vector3 isn't the best for rotations. //could probably use GLM functions.
	//transformComponent->scale = new Vector3(1,1,1) 
	
	//meshComponent->mesh = targetMesh;
}

GameObjectTest::~GameObjectTest() //need to delete any objects/pointers that was created with the 'new' keyword;
{
	std::cout << "CURRENTLY DELETING GAMEOBJECTTEST" << std::endl;
	delete transformComponent; //does this call the destructor? Only one way to find out.
	delete meshComponent;
}

void GameObjectTest::Update()
{
	meshComponent->position = transformComponent->position;
	meshComponent->rotation = transformComponent->rotation;
	meshComponent->scale = transformComponent->scale;
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
