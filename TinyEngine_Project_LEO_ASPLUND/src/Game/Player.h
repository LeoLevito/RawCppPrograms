#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(Vector position);

	void update() override;
	void draw() override;

	void hit(int damage) override
	{
		if (is_invincible())
			return;

		last_hit_time = engCurrentTime();

		health -= damage;
		if (health <= 0)
		{
			destroy();
		}
	}

	float speed = 300.f;

private:
	static constexpr float INVINCIBILITY_TIME = 2.f;
	static constexpr int MAX_HEALTH = 3;

	bool is_invincible()
	{
		return engTimePassedSince(last_hit_time) < INVINCIBILITY_TIME;
	}

	int health = MAX_HEALTH;
	float last_hit_time = -INVINCIBILITY_TIME;
};
