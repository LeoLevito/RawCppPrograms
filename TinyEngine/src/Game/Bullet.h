#pragma once
#include "Actor.h"
class Bullet : public Actor {
public:
	Bullet(Vector position, Vector direction);
	void Update() override;
private:
	static constexpr float SPEED = 1000.f;
	static constexpr float BULLET_LIFETIME = 2.f; //emil's way of doing hardcoded magic numbers. Constexpression is known at compile-time, sorta like a macro but instead of copy-pasting text it copy-pastes the expression/value.

	Vector direction;
	float spawnTime{ 0 };
};

