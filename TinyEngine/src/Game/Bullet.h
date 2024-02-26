#pragma once
#include "Actor.h"
class Bullet : public Actor {
public:
	Bullet(Vector position, Vector direction);
	void Update() override;
private:
	Vector direction;
};

