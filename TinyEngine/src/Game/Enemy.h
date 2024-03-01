#pragma once
#include "Actor.h"
class Enemy : public Actor {
public:
	static int NUMENEMIES; //global variable tied to this class.

	Enemy(Vector position);
	~Enemy(); //destructor, antithesis of constructor.
	void Update() override;
	void Draw() override;
	void Hit(int damage) override {
		health -= damage;
		if (health <= 0) {
			Destroy();
		}
	}

	float speed{ 100.f };
private:
	static constexpr int MAXHEALTH = 100;
	int health = 100;
};

