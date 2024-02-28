#include "Vector.h"

Vector Vector::lerp(Vector a, Vector b, float t) {
	return a + (b - a) * t;
}
