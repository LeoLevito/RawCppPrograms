#pragma once
#include "Vec2.h"
#include <list>
class Node
{
public:
	Vec2 position{};
	int id{};
	std::list<Node> neighbors{};
	Vec2 getPosition(int ide);
	Node() = default;
	Node(Vec2 _position, int _id)
	{
		this->position = _position;
		this->id = _id;
	}
};
