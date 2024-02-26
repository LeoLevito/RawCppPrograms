#pragma once
#include "Actor.h"
class Player : public Actor { //constructors are not inherited.
public:
	Player(Vector position); //thus make own constructor for player.
	void Update() override;
	void Draw() override;
	float speed{ 300.f };
};

