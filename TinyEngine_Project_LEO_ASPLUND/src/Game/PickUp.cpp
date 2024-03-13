#include "PickUp.h"

PickUp::PickUp(Vector position)
	: Actor(position, Vector(20), COLOR_GREEN)
{
	collision_channel = Collision_Channel::PickUp;
}
