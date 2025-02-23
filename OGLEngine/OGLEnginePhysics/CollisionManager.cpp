#include "CollisionManager.h"
#include <iostream>
#include <gtc/constants.hpp> //for getting pi.
#include <numbers> //for getting pi.
#include <cmath> //for getting pi.
#include <gtc/matrix_transform.hpp>
#include <GameObject.h>
#include "RigidbodyComponent.h"
#include "ColliderComponent.h"
#include <thread>
#include <GLFW/glfw3.h>


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

Collider* CollisionManager::AddNewCollider(ColliderType type, GameObject& topParent, ColliderComponent& parent)
{
	switch (type)
	{
	case ColliderType::SphereType:
	{ //curious thing with switch cases, if you wanna initialize a new object, like directionalLight here, you need to have explicit scopes ({}), since otherwise we'd get an uninitialized variable if this case isn't hit. Best to just put this stuff in a function and call that instead. 
		SphereCollider* sphereCollider = new SphereCollider();
		sphereCollider->type = type;
		sphereCollider->parent = &parent;
		sphereColliderVector.push_back(sphereCollider);
		return sphereCollider;
	}
	case ColliderType::BoxType:
	{
		BoxCollider* boxCollider = new BoxCollider();
		boxCollider->type = type;
		boxCollider->parent = &parent;
		boxColliderVector.push_back(boxCollider);
		return boxCollider;
	}
	case ColliderType::MeshType:
	{
		MeshColliderSAT* meshCollider = new MeshColliderSAT();
		meshCollider->type = type;
		meshCollider->topParent = &topParent;
		meshColliderSATVector.push_back(meshCollider);
		return meshCollider;
		break;
	}
	case ColliderType::RaycastType:
	{
		RaycastCollider* raycastCollider = new RaycastCollider();
		raycastCollider->type = type;
		raycastColliderVector.push_back(raycastCollider);
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
		raycastColliderVector.erase(std::remove(raycastColliderVector.begin(), raycastColliderVector.end(), collider));
		break;
	default:
		break;
	}
	delete collider;
}

void CollisionManager::Process()
{
	double lastTime = 0;
	double currentTime = 0;
	double deltaTime = 0;

	while (shouldRun) //what happens if I limit this to say every 50th of a second?
	{
		currentTime = glfwGetTime(); //May or may not be super accurate at the moment.
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//std::this_thread::sleep_for(std::chrono::milliseconds(20));
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
		RaySphereTest();
		RayBoxTest();

		for (int i = 0; i < sphereColliderVector.size(); i++)
		{
			if (sphereColliderVector[i]->parent->myRigidbody != nullptr) //man these checks are slowly getting ridiculous.
			{
				sphereColliderVector[i]->parent->myRigidbody->ApplyGravity(deltaTime);
			}
		}

		for (int i = 0; i < boxColliderVector.size(); i++)
		{
			if (boxColliderVector[i]->parent->myRigidbody != nullptr)
			{
				boxColliderVector[i]->parent->myRigidbody->ApplyGravity(deltaTime);
			}
		}
	}
}

void CollisionManager::SphereSphereTest()
{
	for (int i = 0; i < sphereColliderVector.size(); i++)
	{
		for (int j = i + 1; j < sphereColliderVector.size(); j++)
		{
			if (!sphereColliderVector[j]->hasGotFirstPosition) //takes a little while for a new box collider to fill the corners vector, this if check prevents a vector out of range error that's caused by this function running on a different thread compared to the box collider bounds update.
			{											   //Hmm, maybe I should set it to check if corners.size() == 8, just to be sure?
														   //Or maybe this isn't needed anymore because I run the UpdateBounds function before this one inside the Process() loop of this class?
				return;
			}
			//do collision test.
			float distance = glm::distance(sphereColliderVector[i]->position, sphereColliderVector[j]->position); //apparently you can save a square root by calculating the squared distance and comparing it with a squared radius. I don't think i've done this right now. Might do it in the future.
			float margin = sphereColliderVector[i]->radius + sphereColliderVector[j]->radius; //multithreading error here, when changing one of two sphere colliders to a box collider, this line will try to access the now deleted sphere collider thus causing an error. May be worsened with higher framerates.

			if (distance < margin)
			{
				std::cout << "Sphere-Box collision detected!!" << std::endl;

				//https://www.youtube.com/watch?v=1L2g4ZqmFLQ, and Engine Physics lecture 4 by Martin.
				glm::vec3 collisionNormal = glm::normalize(sphereColliderVector[i]->position - sphereColliderVector[j]->position); //are we sure this is the actual normal?
				glm::vec3 relativeVelocity = sphereColliderVector[i]->parent->myRigidbody->velocity - sphereColliderVector[j]->parent->myRigidbody->velocity;
				float dotVelocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);
				if (dotVelocityAlongNormal < 0) //if moving toward each other.
				{
					float restitution = glm::min(sphereColliderVector[i]->parent->myRigidbody->restitution, sphereColliderVector[j]->parent->myRigidbody->restitution); //something is not right with the restitution, should be only 0-1. as per the video.

					float impulseMagnitude = -(1 + restitution) * glm::dot(relativeVelocity, collisionNormal) / (1.0f / sphereColliderVector[i]->parent->myRigidbody->mass) + (1.0f / sphereColliderVector[j]->parent->myRigidbody->mass);
					glm::vec3 impulseDirection = collisionNormal;

					glm::vec3 impulse = impulseDirection * impulseMagnitude;

					if (!sphereColliderVector[i]->parent->myRigidbody->isKinematic)
					{
						sphereColliderVector[i]->parent->myRigidbody->ApplyVelocity(0.02f, impulse);
						//sphereColliderVector[i]->parent->myRigidbody->velocity += impulse;
					}

					if (!sphereColliderVector[j]->parent->myRigidbody->isKinematic)
					{
						sphereColliderVector[j]->parent->myRigidbody->ApplyVelocity(0.02f, -impulse);
						//sphereColliderVector[j]->parent->myRigidbody->velocity -= impulse;
					}
				}
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

			glm::mat4 transinv = glm::mat4(1.0f);
			transinv = glm::inverse(boxColliderVector[j]->transWithoutScale);
			glm::vec3 localSphereCenter = glm::vec3(transinv * glm::vec4(sphereColliderVector[i]->position, 1.0f)); //put sphere collider's position into box collider's local space using the inverse transform.

			//https://gamedev.stackexchange.com/questions/157100/why-does-this-implementation-of-aabb-sphere-collision-ghost-collide-and-how-can
			//https://gamedev.stackexchange.com/questions/156870/how-do-i-implement-a-aabb-sphere-collision
			//helpful answers from DrewAtWork.
			glm::vec3 q;
			for (int i = 0; i < 3; i++) //find the closest point on box collider in relation to	the sphere's local position.
			{							//could also square the values here as per DrewAtWork's answers to optimize calculations.
				float v = localSphereCenter[i];
				if (v < boxColliderVector[j]->extentsMin[i]) v = boxColliderVector[j]->extentsMin[i]; // v = max( v, b.min[i] )
				if (v > boxColliderVector[j]->extentsMax[i]) v = boxColliderVector[j]->extentsMax[i]; // v = min( v, b.max[i] )
				q[i] = v; //cursed, or I mean, that's one way to do it: set x when i = 0, y when i = 1, z when i = 2.
			}
			//glm::vec3 closestPointOnCubeSurface = glm::clamp(localSphereCenter, boxColliderVector[j]->extentsMin, boxColliderVector[j]->extentsMax); //essentialy does the same thing as for loop above.

			float qDistance = glm::distance(localSphereCenter, q);
			if (qDistance < sphereColliderVector[i]->radius)
			{
				std::cout << "Sphere-Box collision detected!!" << std::endl;
				
				glm::vec3 qInWorldSpace = glm::vec3(boxColliderVector[j]->transWithoutScale * glm::vec4(q, 1.0f)); //(?);

				//https://www.youtube.com/watch?v=1L2g4ZqmFLQ, and Engine Physics lecture 4 by Martin.
				glm::vec3 collisionNormal = glm::normalize(sphereColliderVector[i]->position - qInWorldSpace);
				glm::vec3 relativeVelocity = sphereColliderVector[i]->parent->myRigidbody->velocity - boxColliderVector[j]->parent->myRigidbody->velocity;
				float dotVelocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);
				if (dotVelocityAlongNormal < 0) //if moving toward each other.
				{
					float restitution = glm::min(sphereColliderVector[i]->parent->myRigidbody->restitution, boxColliderVector[j]->parent->myRigidbody->restitution); //something is not right with the restitution, should be only 0-1. as per the video.

					float impulseMagnitude = -(1 + restitution) * glm::dot(relativeVelocity, collisionNormal) / (1.0f / sphereColliderVector[i]->parent->myRigidbody->mass) + (1.0f / boxColliderVector[j]->parent->myRigidbody->mass);
					glm::vec3 impulseDirection = collisionNormal;

					glm::vec3 impulse = impulseDirection * impulseMagnitude;

					if (!sphereColliderVector[i]->parent->myRigidbody->isKinematic)
					{
						sphereColliderVector[i]->parent->myRigidbody->ApplyVelocity(0.02f, impulse);
						//sphereColliderVector[i]->parent->myRigidbody->velocity += impulse;
					}

					if (!boxColliderVector[j]->parent->myRigidbody->isKinematic)
					{
						boxColliderVector[j]->parent->myRigidbody->ApplyVelocity(0.02f, -impulse);
						//boxColliderVector[j]->parent->myRigidbody->velocity -= impulse;
					}
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
				std::cout << "Box-Box collision detected!!" << std::endl;
			}
		}
	}
}

//https://stackoverflow.com/a/62872782
void CollisionManager::RaySphereTest()
{
	for (int i = 0; i < raycastColliderVector.size(); i++)
	{
		for (int j = 0; j < sphereColliderVector.size(); j++) //is this how you're supposed to do it with different collider types?
		{
			glm::vec3 rayDir = glm::normalize(raycastColliderVector[i]->endPoint - raycastColliderVector[i]->startPoint);
			float rayLength = glm::length(raycastColliderVector[i]->endPoint - raycastColliderVector[i]->startPoint); //is this right?
			float projectionLength = glm::clamp(glm::dot(sphereColliderVector[j]->position - raycastColliderVector[i]->startPoint, rayDir), 0.0f, rayLength);

			glm::vec3 closestPointOnLine = raycastColliderVector[i]->startPoint + rayDir * projectionLength;

			float distanceBetweenSphereAndClosestPoint = glm::distance(sphereColliderVector[j]->position, closestPointOnLine);
			if (distanceBetweenSphereAndClosestPoint < sphereColliderVector[j]->radius)
			{
				std::cout << "Ray-Sphere collision detected!!" << std::endl;
			}
		}
	}
}

//https://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/
bool CollisionManager::RayBoxTest() //a mix between the Sphere-Box test and the Ray-Sphere test. Or am I not thinking far enough ahead with this one?
{
	for (int i = 0; i < raycastColliderVector.size(); i++)
	{
		for (int j = 0; j < boxColliderVector.size(); j++)
		{
			if (!boxColliderVector[j]->corners.size() > 0) //takes a little while for a new box collider to fill the corners vector, this if check prevents a vector out of range error that's caused by this function running on a different thread compared to the box collider bounds update.
			{											   //Hmm, maybe I should set it to check if corners.size() == 8, just to be sure?
														   //Or maybe this isn't needed anymore because I run the UpdateBounds function before this one inside the Process() loop of this class?
				return false;
			}
			glm::vec3 rayOrigin = raycastColliderVector[i]->startPoint;
			glm::vec3 rayDirection = glm::normalize(raycastColliderVector[i]->endPoint - raycastColliderVector[i]->startPoint);
			float rayDistance = glm::distance(raycastColliderVector[i]->startPoint, raycastColliderVector[i]->endPoint);
			glm::vec3 aabbMin = boxColliderVector[j]->extentsMin;
			glm::vec3 aabbMax = boxColliderVector[j]->extentsMax;
			glm::mat4 modelMatrix = boxColliderVector[j]->transWithoutScale; //scale would mess things up, just like with the SphereBoxTest().
			float intersectionDistance;

			float tMin = 0.0f;
			float tMax = 100000.0f;

			glm::vec3 OBBpositionWorldSpace = boxColliderVector[j]->position;
			glm::vec3 OBBpositionWorldSpace2 = { modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z };
			glm::vec3 delta = OBBpositionWorldSpace - rayOrigin;

			{ //WOAH!! you can scope variables with the same name in the same function by having extra brackets!!
				glm::vec3 xaxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z); //wat.
				float e = glm::dot(xaxis, delta);
				float f = glm::dot(rayDirection, xaxis);

				if (glm::abs(f) > 0.001f)
				{
					float t1 = (e + aabbMin.x) / f; //beware: don't do division if f is nearly 0.
					float t2 = (e + aabbMax.x) / f; //beware: don't do division if f is nearly 0.

					if (t1 > t2)
					{
						float w = t1;
						t1 = t2;
						t2 = w; //swap t1 and t2 if they're in the wrong order.
					}

					if (t2 < tMax)
					{
						tMax = t2;
					}

					if (t1 > tMin)
					{
						tMin = t1;
					}

					if (tMax < tMin)
					{
						//Not intersecting
						return false;
					}
				}
				else
				{
					if (-e + aabbMin.x > 0.0f || -e + aabbMax.x < 0.0f)
					{
						// "Not intersecting"...
						return false;
					}
				}
			}
			{ //WOAH!! you can scope variables with the same name in the same function by having extra brackets!!
				glm::vec3 yaxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z); //wat.
				float e = glm::dot(yaxis, delta);
				float f = glm::dot(rayDirection, yaxis);

				if (glm::abs(f) > 0.001f)
				{
					float t1 = (e + aabbMin.y) / f; //beware: don't do division if f is nearly 0.
					float t2 = (e + aabbMax.y) / f; //beware: don't do division if f is nearly 0.

					if (t1 > t2)
					{
						float w = t1;
						t1 = t2;
						t2 = w; //swap t1 and t2 if they're in the wrong order.
					}

					if (t2 < tMax)
					{
						tMax = t2;
					}

					if (t1 > tMin)
					{
						tMin = t1;
					}

					if (tMax < tMin)
					{
						//Not intersecting
						return false;
					}
				}
				else
				{
					if (-e + aabbMin.y > 0.0f || -e + aabbMax.y < 0.0f)
					{
						// "Not intersecting"...
						return false;
					}
				}
			}
			{ //WOAH!! you can scope variables with the same name in the same function by having extra brackets!!
				glm::vec3 zaxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z); //wat.
				float e = glm::dot(zaxis, delta);
				float f = glm::dot(rayDirection, zaxis);

				if (glm::abs(f) > 0.001f)
				{
					float t1 = (e + aabbMin.z) / f; //beware: don't do division if f is nearly 0.
					float t2 = (e + aabbMax.z) / f; //beware: don't do division if f is nearly 0.

					if (t1 > t2)
					{
						float w = t1;
						t1 = t2;
						t2 = w; //swap t1 and t2 if they're in the wrong order.
					}

					if (t2 < tMax)
					{
						tMax = t2;
					}

					if (t1 > tMin)
					{
						tMin = t1;
					}

					if (tMax < tMin)
					{
						//Not intersecting
						return false;
					}
				}
				else
				{
					if (-e + aabbMin.z > 0.0f || -e + aabbMax.z < 0.0f)
					{
						// "Not intersecting"...
						return false;
					}
				}
			}

			//actually intersecting
			intersectionDistance = tMin; //should check this, if I can use this to check if it's less than ray distance. Maybe I also need to calculate ray distance here, like I'm doing with ray direction.
			if (intersectionDistance < rayDistance) //collision!
			{
				std::cout << "Ray-Box collision detected!!" << std::endl;
				return true; //okay so RayBoxTest() now works! Everything from different positions on both Ray and Box, as well as rotation and scale on Box. Just one problem, the line that's being tested is infinite in length from the startPoint.
			}
			else
			{
				return false;
			}
		}
	}
}

//https://gamedev.stackexchange.com/questions/44500/how-many-and-which-axes-to-use-for-3d-obb-collision-with-sat/
//helpful code answers from Acegikmo (Freya Holmér!!) and Bas Smit!
bool CollisionManager::FindMaxMinProjectionAB(BoxCollider& boxA, BoxCollider& boxB) //I gotta rename this.
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
	//But where are we colliding?
}

void CollisionManager::DebugGUI()
{
	if (ImGui::Checkbox("Simulate physics", &simulate))
	{
	}
}
