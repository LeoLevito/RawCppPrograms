#pragma once
#include <vector>
#include "Collider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "MeshColliderSAT.h"

struct MinMax
{
public:
	float Min;
	float Max;

	MinMax(float min, float max)
	{
		Min = min;
		Max = max;
	}
private:
};

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	static CollisionManager& Get();

	//vectors of each collider type, like the LightManager has for Lights;

	//add, 
	//remove
	Collider* AddNewCollider(ColliderType type, GameObject& owner);
	void DeleteCollider(ColliderType type, Collider* collider);


	void Process();
	bool shouldRun = true;

	//loop for collision testing. 
	//If SphereColliders > 1, for loop sphere colliders and check if colliding.
	//also need some kind of tagging (like the game named tag) system where we only need to do a collision test between two colliders once per interval. Ruling out pairs if they've already been checked.

	std::vector<SphereCollider*> sphereColliderVector;
	std::vector<BoxCollider*> boxColliderVector;
	std::vector<MeshColliderSAT*> meshColliderSATVector;
	void SphereSphereTest();
	void SphereBoxTest();
	void BoxBoxTest();
	void MeshMeshTest();
	MinMax FindMaxMinProjection(BoxCollider& box, glm::vec3 axis);
	MinMax FindMaxMinProjection(MeshColliderSAT& mesh, glm::vec3 axis);

	int runFindAmount2 = 0;
	int runFindAmount1 = 0;
private:
};

