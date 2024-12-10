#pragma once
#include "Message.h"
#include "glm.hpp"
class RaycastMessage : public Message
{
public:
	RaycastMessage() : Message(MessageType::Raycast) {}
	glm::vec3 startPos;
	glm::vec3 direction;
	float maxDistance;

	//now on page 39 of game engine ui 3 powerpoint.
private:

};

