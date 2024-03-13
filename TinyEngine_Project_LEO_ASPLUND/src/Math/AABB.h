#pragma once
#include "Vector.h"

struct AABB
{
	static AABB from_position_size(Vector position, Vector size);

	AABB() { }
	AABB(Vector in_min, Vector in_max) : min(in_min), max(in_max) { }

	Vector min;
	Vector max;
};

bool aabb_overlap(AABB a, AABB b);
