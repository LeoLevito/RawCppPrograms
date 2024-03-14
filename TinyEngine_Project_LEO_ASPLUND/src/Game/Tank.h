#pragma once
#include "Enemy.h"
class Tank : public Enemy //Add: Tank class, which derives from Enemy class.
{
public:
	Tank(Vector position) : Enemy(position) //Call Enemy constructor. Set health of tank. Set size of tank. Set color of tank. Set speed of tank.
	{
		MAX_HEALTH = 500;
		health = MAX_HEALTH;
		size = { 75.f, 75.f };
		color = COLOR_BLUE;
		speed = 35.f;
	};
};

