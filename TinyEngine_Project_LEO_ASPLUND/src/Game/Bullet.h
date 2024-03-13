#pragma once
#include "Actor.h"

class Bullet : public Actor
{
public:
	Bullet(Vector position);
	void update() override;

	void set_direction(Vector new_direction)
	{
		direction = new_direction;
	}

private:
	static constexpr float SPEED = 1000.f;
	static constexpr float LIFETIME = 2.f;
	static constexpr int DAMAGE = 40;

	Vector direction;
	float spawn_time = 0.f;
};
