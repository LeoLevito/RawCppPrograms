#pragma once
#include "Actor.h"
class Player : public Actor { //constructors are not inherited.
public:
	Player(Vector position); //thus make own constructor for player.
	void Update() override;
	float speed{ 100.f };
};

