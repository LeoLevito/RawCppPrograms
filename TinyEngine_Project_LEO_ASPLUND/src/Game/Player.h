#pragma once
#include "Actor.h"
#include "TimerSystem.h"

class Player : public Actor
{
public:
	Player(Vector position);

	void update() override;
	void draw() override;
	void shoot(); //Add: shoot function becuase I'm using the same behavior in different parts of the player.cpp file.

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

	float timeSinceLastBullet = 0.f; // Add: timeSinceLastBullet variable

	float shootDelay = 0.2f; // Add: shootDelay variable

	static constexpr int MAX_AMMO = 20; // Add: MAX_AMMO variable;
	int ammo = MAX_AMMO;// Add: ammo variable;

	
};
