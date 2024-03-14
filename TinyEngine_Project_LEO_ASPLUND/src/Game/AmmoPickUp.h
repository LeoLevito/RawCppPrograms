#pragma once
#include "PickUp.h"
class AmmoPickUp : public PickUp //Add: AmmoPickUp class, which derives from PickUp class.
{
public:
	AmmoPickUp(Vector position) : PickUp(position) //Call PickUp constructor. Set size of ammo pick up. Set color of ammo pick up. Set collision channel of ammo pick up.
	{
		size = { 15, 15 };
		color = COLOR_YELLOW;
		collision_channel = Collision_Channel::PickUpAmmo;
	};
};

