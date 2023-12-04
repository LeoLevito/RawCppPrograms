#include "Vec2.h"

bool Vec2::isEqualTo(Vec2 otherPosition) {
	if ((x == otherPosition.x) && (y == otherPosition.y)) {
		return true;
	}else{
		return false;
	}
}