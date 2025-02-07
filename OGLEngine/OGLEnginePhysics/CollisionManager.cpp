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

Collider* CollisionManager::AddNewCollider(ColliderType type, GameObject& owner)
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
	case ColliderType::MeshType:
	{
		MeshColliderSAT* meshCollider = new MeshColliderSAT();
		meshColliderSATVector.push_back(meshCollider);
		meshCollider->type = type;
		meshCollider->topParent = &owner;
		return meshCollider;
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
	switch (type)
	{
	case ColliderType::SphereType:
		sphereColliderVector.erase(std::remove(sphereColliderVector.begin(), sphereColliderVector.end(), collider));
		break;
	case ColliderType::BoxType:
		boxColliderVector.erase(std::remove(boxColliderVector.begin(), boxColliderVector.end(), collider));
		break;
	case ColliderType::MeshType:
		meshColliderSATVector.erase(std::remove(meshColliderSATVector.begin(), meshColliderSATVector.end(), collider));
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
			box->UpdateBounds();
		}
		for (auto mesh : meshColliderSATVector) //need to call this here to avoid random threading related error due to clearing of vectors in the function inside this scope.
		{
			mesh->UpdateBounds();
		}
		BoxBoxTest();
		MeshMeshTest();
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
			if (!boxColliderVector[j]->corners.size() > 0)
			{
				return;
			}
			bool colliding = true;

			for(auto normal : boxColliderVector[i]->normalVector)
			{
				auto mm1 = FindMaxMinProjection(*boxColliderVector[i], normal);
				auto mm2 = FindMaxMinProjection(*boxColliderVector[j], normal);

				if (mm1.Max < mm2.Min || mm2.Max < mm1.Min) 
				{
					//not colliding;
					//return false;
					colliding = false;
					//std::cout << "first boxcollider is not colliding" << std::endl;
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
					colliding = false;
					//std::cout << "second boxcollider is not colliding" << std::endl;
				}
			}

			//colliding
			//return true;
			if (colliding == true)
			{
				std::cout << "collision detected!!" << std::endl;
			}
		}
	}

	
}

void CollisionManager::MeshMeshTest()
{
	for (int i = 0; i < meshColliderSATVector.size(); i++)
	{
		for (int j = i + 1; j < meshColliderSATVector.size(); j++)
		{
			if (!meshColliderSATVector[j]->corners.size() > 0)
			{
				return;
			}
			bool colliding = true;

			for (auto normal : meshColliderSATVector[i]->normalVector)
			{
				auto mm1 = FindMaxMinProjection(*meshColliderSATVector[i], normal);
				auto mm2 = FindMaxMinProjection(*meshColliderSATVector[j], normal);

				if (mm1.Max < mm2.Min || mm2.Max < mm1.Min)
				{
					//not colliding;
					//return false;
					colliding = false;
					//std::cout << "first boxcollider is not colliding" << std::endl;
				}
			}

			for (auto normal : meshColliderSATVector[j]->normalVector)
			{
				auto mm1 = FindMaxMinProjection(*meshColliderSATVector[i], normal);
				auto mm2 = FindMaxMinProjection(*meshColliderSATVector[j], normal);

				if (mm1.Max < mm2.Min || mm2.Max < mm1.Min)
				{
					//not colliding;
					//return false;
					colliding = false;
					//std::cout << "second boxcollider is not colliding" << std::endl;
				}
			}

			//colliding
			//return true;
			if (colliding == true)
			{
				std::cout << "collision detected!!" << std::endl;
			}
		}
	}
}

MinMax CollisionManager::FindMaxMinProjection(BoxCollider& box, glm::vec3 axis)
{
	auto projection = glm::dot(box.corners[0], axis);
	auto min = projection;
	auto max = projection;

	for (int i = 1; i < box.corners.size(); i++)
	{
		projection = glm::dot(box.corners[i], axis);
		max = max > projection ? max : projection;
		min = min < projection ? min : projection;
	}
	return MinMax(min, max);
}

MinMax CollisionManager::FindMaxMinProjection(MeshColliderSAT& mesh, glm::vec3 axis)
{
	auto projection = glm::dot(mesh.corners[0], axis);
	auto min = projection;
	auto max = projection;

	for (int i = 1; i < mesh.corners.size(); i++)
	{
		projection = glm::dot(mesh.corners[i], axis);
		max = max > projection ? max : projection;
		min = min < projection ? min : projection;
	}
	return MinMax(min, max);
}
