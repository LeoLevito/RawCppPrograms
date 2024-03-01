#include "PickUp.h"

PickUp::PickUp(Vector position)
	: Actor(position, Vector(20), COLOR_GREEN)
{
	collisionChannel = CollisionChannel::PickUp;
}