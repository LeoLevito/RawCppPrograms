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


		for (auto box : boxColliderVector) //need to call this here to avoid random threading related error due to clearing of vectors in the function inside this scope.
		{
			box->UpdateBounds();
		}
		for (auto mesh : meshColliderSATVector) //need to call this here to avoid random threading related error due to clearing of vectors in the function inside this scope.
		{
			mesh->UpdateBounds();
		}
		SphereSphereTest();
		BoxBoxTest();
		SphereBoxTest();
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
	for (int i = 0; i < sphereColliderVector.size(); i++)
	{
		for (int j = 0; j < boxColliderVector.size(); j++) //is this how you're supposed to do it with different collider types?
		{
			if (!boxColliderVector[j]->corners.size() > 0) //takes a little while for a new box collider to fill the corners vector, this if check prevents a vector out of range error that's caused by this function running on a different thread compared to the box collider bounds update.
			{											   //Hmm, maybe I should set it to check if corners.size() == 8, just to be sure?
														   //Or maybe this isn't needed anymore because I run the UpdateBounds function before this one inside the Process() loop of this class?
				return;
			}

			//https://www.youtube.com/watch?v=wVhSQHKvBW4
			//https://gamedeveloperjourney.blogspot.com/2009/04/point-plane-collision-detection.html
			bool insideVertex = false;
			bool insideFace = false;
			bool insideEdge = false;
			bool inside = false;
			//may need to translate sphere position so its position is in the Box's local space.
			//glm::vec3 closestCornerOnBoxInRelationToSpherePosition;
			float lastClosestDistance = 99999999.0f;
			for(glm::vec3 corner : boxColliderVector[j]->corners)
			{
				float currentDistance = glm::distance(sphereColliderVector[i]->position, corner);
				if (currentDistance < lastClosestDistance)
				{
					//closestCornerOnBoxInRelationToSpherePosition = corner;
					lastClosestDistance = currentDistance;
				}
			}

			if (lastClosestDistance < sphereColliderVector[i]->radius) //this only covers corners and not faces or edges. 
			{
				//could swap > to < for return false early out.
				insideVertex = true;
				std::cout << "Sphere and Box are colliding at a vertex!" << std::endl;
			}

			SphereCollider* currentSphereCollider = sphereColliderVector[i];
			for (int i = 0; i < 6; i++) //we know box has 6 face normals.
			{
				float planeDistance = -glm::dot(boxColliderVector[j]->averageVector[i], boxColliderVector[j]->normalVector[i]);

				float ppd = glm::dot(boxColliderVector[j]->normalVector[i], currentSphereCollider->position) + planeDistance;

				if (ppd > currentSphereCollider->radius) //could be other way around.
				{
					//do early out
					//return false
					insideFace = false; //false actually
					std::cout << "Sphere and Box are NOT colliding at a face!" << std::endl;   
				}
				else 
				{
					insideFace = true; //true??
					std::cout << "Sphere and Box are colliding at a face!" << std::endl;
				}
			}
		}
	}
}

//https://textbooks.cs.ksu.edu/cis580/04-collisions/04-separating-axis-theorem/ is good for 2D, but not 3D!
void CollisionManager::BoxBoxTest()
{
	for (int i = 0; i < boxColliderVector.size(); i++)
	{
		for (int j = i + 1; j < boxColliderVector.size(); j++)
		{
			if (!boxColliderVector[j]->corners.size() > 0) //takes a little while for a new box collider to fill the corners vector, this if check prevents a vector out of range error that's caused by this function running on a different thread compared to the box collider bounds update.
			{											   //Hmm, maybe I should set it to check if corners.size() == 8, just to be sure?
														   //Or maybe this isn't needed anymore because I run the UpdateBounds function before this one inside the Process() loop of this class?
				return;
			}

			bool colliding = FindMaxMinProjectionAB(*boxColliderVector[i], *boxColliderVector[j]);

			if (colliding == true)
			{
				std::cout << "collision detected!!" << std::endl;
			}
		}
	}
}

//https://gamedev.stackexchange.com/questions/44500/how-many-and-which-axes-to-use-for-3d-obb-collision-with-sat/
//helpful code answers from Acegikmo (Freya Holmér!!) and Bas Smit!
bool CollisionManager::FindMaxMinProjectionAB(BoxCollider& boxA, BoxCollider& boxB)
{
	std::vector<glm::vec3> axes;
	axes.push_back(boxA.right);
	axes.push_back(boxA.up);
	axes.push_back(boxA.forward);

	axes.push_back(boxB.right);
	axes.push_back(boxB.up);
	axes.push_back(boxB.forward);

	axes.push_back(glm::cross(boxA.right, boxB.right));
	axes.push_back(glm::cross(boxA.right, boxB.up));
	axes.push_back(glm::cross(boxA.right, boxB.forward));
	axes.push_back(glm::cross(boxA.up, boxB.right));
	axes.push_back(glm::cross(boxA.up, boxB.up));
	axes.push_back(glm::cross(boxA.up, boxB.forward));
	axes.push_back(glm::cross(boxA.forward, boxB.right));
	axes.push_back(glm::cross(boxA.forward, boxB.up));
	axes.push_back(glm::cross(boxA.forward, boxB.forward));

	for (const auto& axis : axes)
	{
		if (axis.x == 0 && axis.y == 0 && axis.z == 0) //if cross product is (0,0,0), do early out. A head-on face-to-face collision has occured between the two box colliders.
		{
			//https://gamedev.stackexchange.com/questions/191240/how-do-i-resolve-a-collision-in-sat-3d-when-the-projection-axis-is-zero
			//The answer in this question helped me try returning true here, which fixed my issue with face to face head on collisions not working. 
			//Though I believe the answer says that he had to do the opposite. Oh well, Box-Box collision testing finally works now!
			return true;
		}

		float projectionA = glm::dot(boxA.corners[0], axis);
		float minA = projectionA;
		float maxA = projectionA;

		float projectionB = glm::dot(boxB.corners[0], axis);
		float minB = projectionB;
		float maxB = projectionB;

		for (int i = 1; i < 8; i++) //i starts from 1 because we need to (and have already) set min and max first. We also know every box collider has 8 corners/vertices, hence we iterate i up to 8. 
		{
			projectionA = glm::dot(boxA.corners[i], axis);
			maxA = maxA > projectionA ? maxA : projectionA;
			minA = minA < projectionA ? minA : projectionA;

			projectionB = glm::dot(boxB.corners[i], axis);
			maxB = maxB > projectionB ? maxB : projectionB;
			minB = minB < projectionB ? minB : projectionB;
		}
		MinMax mm1{ minA,maxA };
		MinMax mm2{ minB,maxB };

		if (mm1.Max < mm2.Min || mm2.Max < mm1.Min) //separation check, do early out if condition is met.
		{
			//not colliding, we've found the separating axis / separating plane!
			return false;
		}
	}
	return true; //we didn't find a separating axis / separating plane, the two box colliders must be colliding!
}
