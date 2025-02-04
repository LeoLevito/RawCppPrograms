#include "CollisionManager.h"
#include <iostream>

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

CollisionManager& CollisionManager::Get()
{
	static CollisionManager instance;
	return instance;
}

Collider* CollisionManager::AddNewCollider(ColliderType type)
{
	switch (type)
	{
	case ColliderType::SphereType:
	{ //curious thing with switch cases, if you wanna initialize a new object, like directionalLight here, you need to have explicit scopes ({}), since otherwise we'd get an uninitialized variable if this case isn't hit. Best to just put this stuff in a function and call that instead. 
		SphereCollider* sphereCollider = new SphereCollider();
		sphereColliderVector.push_back(sphereCollider);
		sphereCollider->type = type;
		return sphereCollider;
	}
	case ColliderType::BoxType:
	{
		BoxCollider* boxCollider = new BoxCollider();
		boxColliderVector.push_back(boxCollider);
		boxCollider->type = type;
		return boxCollider;
	}
	case ColliderType::RaycastType:
	{
		break;
	}
	default:
		break;
	}
}

void CollisionManager::DeleteCollider(ColliderType type, Collider* collider)
{
	switch (type)
	{
	case ColliderType::SphereType:
		sphereColliderVector.erase(std::remove(sphereColliderVector.begin(), sphereColliderVector.end(), collider));
		break;
	case ColliderType::BoxType:
		boxColliderVector.erase(std::remove(boxColliderVector.begin(), boxColliderVector.end(), collider));
		break;
	case ColliderType::RaycastType:

		break;
	default:
		break;
	}
	delete collider;
}

void CollisionManager::Process()
{
	while (shouldRun)
	{
		//ProcessMessages();
		SphereSphereTest();
		BoxBoxTest();
	}
}

void CollisionManager::SphereSphereTest()
{
	for (int i = 0; i < sphereColliderVector.size(); i++)
	{
		for (int j = i + 1; j < sphereColliderVector.size(); j++)
		{
			//do collision test.
			float distance = glm::distance(sphereColliderVector[i]->position, sphereColliderVector[j]->position); //apparently you can save a square root by calculating the squared distance and comparing it with a squared radius. I don't think i've done this right now. Might do it in the future.
			float margin = sphereColliderVector[i]->radius + sphereColliderVector[j]->radius;

			if (distance < margin)
			{
				std::cout << "Distance: " << distance << " margin: " << margin << " two spheres currently colliding." << std::endl;
			}
		}
	}
}

void CollisionManager::SphereBoxTest()
{
	//for (int i = 0; i < sphereColliderVector.size(); i++)
	//{
	//	for (int j = 0; j < boxColliderVector.size(); j++) //is this how you're supposed to do it with different collider types?
	//	{
	//		//do collision test.

	//	}
	//}
}

void CollisionManager::BoxBoxTest()
{
	for (int i = 0; i < boxColliderVector.size(); i++)
	{
		for (int j = i + 1; j < boxColliderVector.size(); j++)
		{
			//do collision test.
			//but how to do collision test with two boxes that are rotated?
			bool colliding;



			if (colliding)
			{
				std::cout << " two boxes currently colliding." << std::endl;
			}
		}
	}
}
