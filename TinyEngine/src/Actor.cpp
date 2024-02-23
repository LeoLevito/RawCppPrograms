#include "Actor.h"

void Actor::Draw() { //defining a function that's inside this class
	
	engSetDrawColor(color);
	engFillRect((int)position.x - size.x / 2, (int)position.y - size.y / 2, (int)size.x, (int)size.y); // - size.x / 2 means center of actor or player.
}