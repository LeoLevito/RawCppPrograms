#pragma once
#include "Actor.h"

class Enemy : public Actor
{
public:
	static int NUM_ENEMIES;

	Enemy(Vector position);
	~Enemy() override;
	void update() override;
	void draw() override;

	void hit(int damage) override
	{
		health -= damage;
		if (health <= 0)
		{
			destroy();
		}
	}

	float speed = 50.f;

protected: //Add: protected access modifier
	int MAX_HEALTH = 100; //change: moved MAX_HEALTH to protected. Removed static constexpr. Now possible to access in deriving classes.
	int health = MAX_HEALTH; //change: moved to protected. Now possible to access in deriving classes.
private:

};

extern int NUM_ENEMIES;
