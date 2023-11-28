#include "Node.h"
#include <iostream>
Vec2 Node::getPosition(int ide) {
	if (ide == id) {
		std::cout << " RETURN POSITION!";
		return position;
	}
	else {
		Vec2 lasarous{ 0,0 };
		std::cout << " RETURN LASAROUS!";
		return lasarous;
	}
}