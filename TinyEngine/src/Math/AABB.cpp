#include "AABB.h"

AABB AABB::fromPositionSize(Vector position, Vector size) //definition of struct static function.
{
	return AABB( //utilizing constructor
		position - size * 0.5f,
		position + size * 0.5f
	);
}

bool aabbOverlap(AABB a, AABB b)
{
	return
		a.max.x > b.min.x && b.max.x > a.min.x && // x-axis check.
		a.max.y > b.min.y && b.max.y > a.min.y; // y-axis check.
}


