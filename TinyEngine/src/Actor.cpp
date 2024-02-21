#include "Actor.h"

void Actor::Draw() { //defining a function that's inside this class
	
	engSetDrawColor(color);
	engFillRect((int)position.x, (int)position.y, (int)size.x, (int)size.y);
}