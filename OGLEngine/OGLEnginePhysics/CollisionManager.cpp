#include "CollisionManager.h"
#include <iostream>
#include <gtc/constants.hpp> //for getting pi.
#include <numbers> //for getting pi.
#include <cmath> //for getting pi.
#include <gtc/matrix_transform.hpp>


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

		if (boxColliderVector.size() > 0)
		{
			for (auto box : boxColliderVector) //need to call this here to avoid random threading related error due to clearing of vectors in the function inside this scope.
			{
				box->UpdateBounds();
			}
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
















			//BRUH HOW DO I CONVERT SPHERE WORLD POSITION TO BOX LOCAL SPACE? OR PROJECT SPHERE POSITION COORDINATES TO EACH AXIS OF BOX EXTENTS?
			//this is literally the only thing that I need for it to work unless Martin has actually written something wrong with the clamp part.
			glm::vec3 currentSphereCenter = sphereColliderVector[i]->position;
			glm::mat4 transinv = glm::mat4(1.0f);
			transinv = glm::inverse(boxColliderVector[j]->trans); //inverse

			glm::vec3 localSphereCenter = glm::vec3(transinv * glm::vec4(sphereColliderVector[i]->position, 1.0f));
			//std::cout << localSphereCenter.x << " " << localSphereCenter.y << " " << localSphereCenter.z << " position of sphere in box local space. Right?" << std::endl;
		
			glm::vec3 closestPointOnCubeSurface = glm::clamp(localSphereCenter, boxColliderVector[j]->extentsMin, boxColliderVector[j]->extentsMax); 
			glm::vec3 mama = glm::max(boxColliderVector[j]->extentsMin - localSphereCenter, localSphereCenter - boxColliderVector[j]->extentsMax);
			float dx = glm::clamp(boxColliderVector[j]->extentsMin.x - localSphereCenter.x, 0.0f, localSphereCenter.x - boxColliderVector[j]->extentsMax.x); //clamp is wrong.
			float dy = glm::clamp(boxColliderVector[j]->extentsMin.y - localSphereCenter.y, 0.0f, localSphereCenter.y - boxColliderVector[j]->extentsMax.y);
			float dz = glm::clamp(boxColliderVector[j]->extentsMin.z - localSphereCenter.z, 0.0f, localSphereCenter.z - boxColliderVector[j]->extentsMax.z);
			glm::vec3 result = { dx, dy, dz };
			//glm::vec3 closestPointOnCubeSurface = glm::clamp(boxColliderVector[j]->extentsMin, -boxColliderVector[j]->extents, boxColliderVector[j]->extents);

			float distanceToClosestPoint = glm::distance(sphereColliderVector[i]->position, closestPointOnCubeSurface);
			if (distanceToClosestPoint < sphereColliderVector[i]->radius)
			{
				//std::cout << "Sphere-Box collision detected!!" << std::endl;
			}


			float sqDist = 0.0f;
			for (int i = 0; i < 3; i++)
			{
				float v = localSphereCenter[i]; //Cursed but I kinda like it.
				if (v < boxColliderVector[j]->extentsMin[i]) sqDist += (boxColliderVector[j]->extentsMin[i] - v) * (boxColliderVector[j]->extentsMin[i] - v); //the extents are still so wrong.
				if (v > boxColliderVector[j]->extentsMax[i]) sqDist += (v - boxColliderVector[j]->extentsMax[i]) * (v - boxColliderVector[j]->extentsMax[i]);
			}

			glm::vec3 q;
			for (int i = 0; i < 3; i++) 
			{
				float v = currentSphereCenter[i];
				if (v < boxColliderVector[j]->extentsMin[i]) v = boxColliderVector[j]->extentsMin[i]; // v = max( v, b.min[i] )
				if (v > boxColliderVector[j]->extentsMax[i]) v = boxColliderVector[j]->extentsMax[i]; // v = min( v, b.max[i] )
				q[i] = v; //cursed, x when i = 0, y when i = 1, z when i = 2.
			}
			float qDistance = glm::distance(sphereColliderVector[i]->position, q);
			if (qDistance < sphereColliderVector[i]->radius) //POSITION AND SCALE OF BOX WORKS! BUT NOT ROTATION!
			{
				std::cout << "Sphere-Box collision detected!!" << std::endl; 
			}
			std::cout << "old distance result:" << distanceToClosestPoint <<std::endl;
			std::cout << "NEW distance result:" << qDistance << std::endl;

			//BRUH WHY DO I I HAVE TO WORK WITH EXTENTS!!!! Nothing fucking works, no one tells you how to set up extents.



			auto xyz = glm::max(boxColliderVector[j]->extentsMin, glm::min(localSphereCenter, boxColliderVector[j]->extentsMax));
			auto xyzdistance = glm::sqrt((xyz.x - localSphereCenter.x) * (xyz.x - localSphereCenter.x) +
										 (xyz.y - localSphereCenter.y) * (xyz.y - localSphereCenter.y) +
										 (xyz.z - localSphereCenter.z) * (xyz.z - localSphereCenter.z));
			std::cout << "Nin distance result:" << xyzdistance << std::endl;





			////https://www.youtube.com/watch?v=wVhSQHKvBW4
			////https://gamedeveloperjourney.blogspot.com/2009/04/point-plane-collision-detection.html
			//bool insideVertex = false;
			//bool insideFace = false;
			//bool insideEdge = false;
			//bool inside = false;
			////may need to translate sphere position so its position is in the Box's local space.
			////glm::vec3 closestCornerOnBoxInRelationToSpherePosition;
			//float lastClosestDistance = 99999999.0f;
			//for(glm::vec3 corner : boxColliderVector[j]->corners)
			//{
			//	float currentDistance = glm::distance(sphereColliderVector[i]->position, corner);
			//	if (currentDistance < lastClosestDistance)
			//	{
			//		//closestCornerOnBoxInRelationToSpherePosition = corner;
			//		lastClosestDistance = currentDistance;
			//	}
			//}

			//if (lastClosestDistance < sphereColliderVector[i]->radius) //this only covers corners and not faces or edges. 
			//{
			//	//could swap > to < for return false early out.
			//	insideVertex = true;
			//	std::cout << "Sphere and Box are colliding at a vertex!" << std::endl;
			//}

			//SphereCollider* currentSphereCollider = sphereColliderVector[i];
			//for (int i = 0; i < 6; i++) //we know box has 6 face normals.
			//{
			//	float planeDistance = -glm::dot(boxColliderVector[j]->averageVector[i], boxColliderVector[j]->normalVector[i]);

			//	//float ppd = glm::dot(boxColliderVector[j]->normalVector[i], currentSphereCollider->position) + planeDistance; //maybe this should take into account the radius, and we swap this to be the end point.
			//	float ppdstart = glm::dot(boxColliderVector[j]->normalVector[i], boxColliderVector[j]->position) + planeDistance; //maybe this should take into account the radius, and we swap this to be the end point.

			//	int PlaneFront = 0;
			//	int PlaneBack = 1;
			//	int OnPlane = 2;

			//	int pStartLoc = -1;
			//	int pDestLoc = -1;
			//	//The trick is realizing that a collision only occurs if these two positions have different locations in relation to the plane -- 
			//	//if they're both in front of or both behind the plane, no collision occurred and we can return from the function.
			//	if (ppdstart > 0.0f)
			//	{
			//		pStartLoc = PlaneFront;
			//	}
			//	else if (ppdstart < 0.0f)
			//	{
			//		pStartLoc = PlaneBack;
			//	}
			//	else
			//	{
			//		pStartLoc = OnPlane;
			//	}

			//	float ppddest = glm::dot(boxColliderVector[j]->normalVector[i], currentSphereCollider->position) + planeDistance;

			//	if (ppddest > 0.0f)
			//	{
			//		pDestLoc = PlaneFront;
			//	}
			//	else if (ppddest < 0.0f)
			//	{
			//		pDestLoc = PlaneBack;
			//	}
			//	else
			//	{
			//		pDestLoc = OnPlane;
			//	}

			//	if (pStartLoc == pDestLoc) // if these have the same value, i.e. both being in front or both being behind or both being exactly on the plane, no collision has occured.
			//	{
			//		//std::cout << "no collision has occured between the start and end points!" << std::endl;
			//		//return false; //no collision occured.
			//	}
			//	else
			//	{
			//		//std::cout << "A collision has occured between the start and end points!" << std::endl;

			//	}

			//	glm::vec3 ray = glm::normalize(boxColliderVector[j]->position - currentSphereCollider->position);

			//	auto t = -(planeDistance + glm::dot(boxColliderVector[j]->normalVector[i], currentSphereCollider->position)) / glm::dot(boxColliderVector[j]->normalVector[i], ray);
			//	glm::vec3 intersect = currentSphereCollider->position + (ray * t);

			//	glm::vec3 v1 = glm::normalize(intersect - boxColliderVector[j]->faceCornerVector[i][0]); //vector within vector is cursed.
			//	glm::vec3 v2 = glm::normalize(intersect - boxColliderVector[j]->faceCornerVector[i][1]); //vector within vector is cursed.
			//	glm::vec3 v3 = glm::normalize(intersect - boxColliderVector[j]->faceCornerVector[i][2]); //vector within vector is cursed.
			//	glm::vec3 v4 = glm::normalize(intersect - boxColliderVector[j]->faceCornerVector[i][3]); //vector within vector is cursed.

			//	float thetaSum = glm::acos(glm::dot(v1, v2))
			//		+ glm::acos(glm::dot(v2, v3))
			//		+ glm::acos(glm::dot(v3, v4))
			//		+ glm::acos(glm::dot(v4, v1));

			//	float threeSixty = fabs(thetaSum - (2 * myPi));
			//	if (threeSixty < 0.1f) //glm::atan(1)*4 = pi.
			//	{
			//		//return true;
			//		std::cout << "A collision has occured inside the plane!" << std::endl;
			//	}
			//	else
			//	{
			//		//return false;
			//	}

			//	//std::cout << intersect.x << " " << intersect.y << " " << intersect.z << std::endl;

			//	if (ppdstart > currentSphereCollider->radius) //could be other way around.
			//	{
			//		//do early out
			//		//return false
			//		insideFace = false; //false actually
			//		//std::cout << "Sphere and Box are NOT colliding at a face!" << std::endl;   
			//	}
			//	else 
			//	{
			//		insideFace = true; //true??
			//		//std::cout << "Sphere and Box are colliding at a face!" << std::endl;
			//	}
			//}
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
				std::cout << "Box-Box collision detected!!" << std::endl;
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
