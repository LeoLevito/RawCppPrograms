#pragma once
#include"Vector.h"
struct AABB{ //axis aligned boxes are used right now for actors, so we only need to do simple collision checks.
	static AABB fromPositionSize(Vector position, Vector size);

	AABB() {}
	AABB(Vector min, Vector max) : min(min), max(max) {}
	Vector min;
	Vector max;
};

bool aabbOverlap(AABB a, AABB b);