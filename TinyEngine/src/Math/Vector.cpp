#include "Vector.h"
#include "Engine/TinyEngine.h"
#include "Math/Math.h"


Vector Vector::lerp(Vector a, Vector b, float t) {
	return a + (b - a) * t;
}

Vector Vector::randomPointOnCircle(float radius)
{
	float angle = engRandomF() * Math::TAU;
	return Vector(sinf(angle), cosf(angle)) * radius;
}
