#pragma once
#include "Engine/TinyEngine.h"
#include <cmath>

struct Vector
{
	constexpr Vector() { }
	constexpr Vector(float scalar)
		: x(scalar), y(scalar) { }
	constexpr Vector(float x, float y)
		: x(x), y(y) { }

	void normalize()
	{
		float lengthSqrd = (x * x + y * y);
		if (lengthSqrd < 0.0001f)
			return;

		float length = sqrtf(lengthSqrd);
		x = x / length;
		y = y / length;
	}

	void operator+=(Vector rhs) { x += rhs.x; y += rhs.y; }

	float x = 0.f;
	float y = 0.f;

	static Vector lerp(Vector a, Vector b, float t);
	static Vector random_point_on_circle(float radius);
};

inline Vector operator+(Vector lhs, Vector rhs) { return Vector(lhs.x + rhs.x, lhs.y + rhs.y); }
inline Vector operator-(Vector lhs, Vector rhs) { return Vector(lhs.x - rhs.x, lhs.y - rhs.y); }
inline Vector operator*(Vector lhs, float rhs) { return Vector(lhs.x * rhs, lhs.y * rhs); }
inline Vector operator*(float lhs, Vector rhs) { return Vector(rhs.x * lhs, rhs.y * lhs); }