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

Collider* CollisionManager::AddNewCollider(ColliderType type, ColliderComponent& componentRef)
{
	switch (type)
	{
	case ColliderType::SphereType:
	{ //curious thing with switch cases, if you wanna initialize a new object, like directionalLight here, you need to have explicit scopes ({}), since otherwise we'd get an uninitialized variable if this case isn't hit. Best to just put this stuff in a function and call that instead. 
		SphereCollider* sphereCollider = new SphereCollider(componentRef);
		sphereColliderVector.push_back(sphereCollider);
		sphereCollider->type = type;
		return sphereCollider;
	}
	case ColliderType::BoxType:
	{
		BoxCollider* boxCollider = new BoxCollider(componentRef);
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
		//SphereSphereTest();

		for (auto box : boxColliderVector) //need to call this here to avoid random threading related error due to clearing of vectors in the function inside this scope.
		{
			//box->SetPosition(box->compRef.); //BRUH WHAT IS THIS!!!! Maybe the compRef goes infinitely deep, like the box has a component reference which has a reference to the box which has a reference to the component...... and so on...
		}

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

//https://textbooks.cs.ksu.edu/cis580/04-collisions/04-separating-axis-theorem/
void CollisionManager::BoxBoxTest()
{
	for (int i = 0; i < boxColliderVector.size(); i++)
	{
		for (int j = i + 1; j < boxColliderVector.size(); j++)
		{
			for(auto normal : boxColliderVector[i]->normalVector)
			{
				auto mm1 = FindMaxMinProjection(*boxColliderVector[i], normal);
				auto mm2 = FindMaxMinProjection(*boxColliderVector[j], normal);

				if (mm1.Max < mm2.Min || mm2.Max < mm1.Min) 
				{
					//not colliding;
					//return false;
					std::cout << "first boxcollider is not colliding" << std::endl;
				}
			}

			for (auto normal : boxColliderVector[j]->normalVector)
			{
				auto mm1 = FindMaxMinProjection(*boxColliderVector[i], normal);
				auto mm2 = FindMaxMinProjection(*boxColliderVector[j], normal);

				if (mm1.Max < mm2.Min || mm2.Max < mm1.Min)
				{
					//not colliding;
					//return false;
					std::cout << "second boxcollider is not colliding" << std::endl;
				}
			}

			//colliding
			//return true;
			std::cout << "collision detected?" << std::endl;
		}
	}
}

MinMax CollisionManager::FindMaxMinProjection(BoxCollider& box, glm::vec3 axis)
{
	auto projection = glm::dot(box.corners[0], axis);
	auto min = projection;
	auto max = projection;
	runFindAmount1++;
	//std::cout << "Function call: " << runFindAmount1 << 
	//	" corners size: " << box.corners.size() <<
	//	std::endl;
	for (int i = 1; i < box.corners.size(); i++)
	{
		runFindAmount2++;
		//std::cout << "For loop: " << runFindAmount2 << 
		//	" corner: " << box.corners[i].x << " " << box.corners[i].y << " " << box.corners[i].z <<
		//	" axis: " << axis.x << " " << axis.y << " " << axis.z <<
		//	" index i: " << i <<
		//	std::endl;
		projection = glm::dot(box.corners[i], axis);
		max = max > projection ? max : projection;
		min = min < projection ? min : projection;
	}
	return MinMax(min, max);
}
