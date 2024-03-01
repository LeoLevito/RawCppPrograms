#pragma once
#include "Actor.h"
class Player : public Actor { //constructors are not inherited.
public:
	Player(Vector position); //thus make own constructor for player.
	void Update() override;
	void Draw() override;

	void Hit(int damage) override {
		if (isInvincible()) {
			return;
		}

		lastHitTime = engCurrentTime();
		health -= damage;
		if (health <= 0) {
			Destroy();
		}
	}
	float speed{ 300.f };

private:
	static constexpr float INVINCIBILITYTIME{ 2.f };
	static constexpr int MAXHEALTH{ 3 };
	bool isInvincible() {
		return engTimePassedSince(lastHitTime) < INVINCIBILITYTIME;
	}
	int health{ 3 };
	float lastHitTime{ -INVINCIBILITYTIME }; //-invincibilitytime prevents invincibilty at startup
};

