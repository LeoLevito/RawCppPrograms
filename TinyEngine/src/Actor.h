#pragma once
#include "Engine/TinyEngine.h"
#include "Math/Vector.h"

enum class CollisionChannel { //newer style enum, enum class instead of just enum. otherwise the enums will be global, which may get cluttery.
	None,
	Player,
	Enemy
};

class Actor { //members of class are private by default, virtually no difference between struct and class except this. 
public: //everything after this will be public
	//Actor() {}; //default constructor has no parameters.
	Actor(Vector position, Vector size, Color color)
		: position(position), size(size), color(color) {}

	virtual void Update() {};
	virtual void Draw(); //member function (function of this class

	void Destroy() { isDestroyed = true; }
	bool getIsDestroyed() { return isDestroyed; }

	Vector position;
	Vector size{32};
	Color color{ COLOR_WHITE };

	CollisionChannel collisionChannel = CollisionChannel::Player;

private:
	bool isDestroyed{ false };
};

