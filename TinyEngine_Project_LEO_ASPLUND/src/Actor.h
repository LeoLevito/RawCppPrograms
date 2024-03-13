#pragma once
#include "Math/Vector.h"
#include "Engine/TinyEngine.h"

enum class Collision_Channel
{
	None,
	Player,
	Enemy,
	PickUp
};

class Actor
{
public:
	Actor(Vector position, Vector size, Color color)
		: position(position), size(size), color(color) { }
	virtual ~Actor() { }

	virtual void update() {}
	virtual void draw();

	virtual void hit(int damage) { destroy(); }

	void destroy() { is_destroyed = true; }
	bool get_is_destroyed() { return is_destroyed; }

	Vector position;
	Vector size = Vector(32);
	Color color = COLOR_WHITE;

	Collision_Channel collision_channel = Collision_Channel::None;

private:
	bool is_destroyed = false;
};