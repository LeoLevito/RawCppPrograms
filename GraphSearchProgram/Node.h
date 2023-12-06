#pragma once
#include "Vec2.h"

class Node{
public:
	Vec2 position{};
	int id{};
	Node() = default;
	Node(Vec2 _position, int _id){
		this->position = _position;
		this->id = _id;
	}
};