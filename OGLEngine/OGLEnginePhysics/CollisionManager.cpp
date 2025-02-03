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
		break;
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
	//switch (type)
	//{
	//case ColliderType::SphereType:
	//	sphereColliderVector.erase(std::remove(sphereColliderVector.begin(), sphereColliderVector.end(), collider));
	//	for (int i = 0; i < sphereColliderVector.size(); i++)
	//	{
	//		//do stuff if needed.
	//	}
	//	break;
	//case ColliderType::BoxType:

	//	break;
	//case ColliderType::RaycastType:

	//	break;
	//default:
	//	break;
	//}
	delete collider;
}

void CollisionManager::Process()
{
	while (shouldRun)
	{
		//ProcessMessages();
		SphereSphereTest();
	}
}

void CollisionManager::SphereSphereTest()
{
	for (int i = 0; i < sphereColliderVector.size(); i++)
	{
		for (int j = i + 1; j < sphereColliderVector.size(); j++)
		{
			//do collision test.
			float distance = glm::distance(sphereColliderVector[i]->position, sphereColliderVector[j]->position);
			float margin = sphereColliderVector[i]->radius + sphereColliderVector[j]->radius;

			if (distance < margin)
			{
				std::cout << "Distance: " << distance << " margin: " << margin << " currently colliding." << std::endl;
			}
		}
	}
}
