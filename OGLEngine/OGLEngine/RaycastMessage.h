#pragma once
#include "Message.h"
#include "glm.hpp"
class RaycastMessage : public Message
{
public:
	RaycastMessage() : Message(MessageType::Raycast, "shooting raycast") {}
	glm::vec3 startPos;
	glm::vec3 direction;
	float maxDistance;

	~RaycastMessage();
	//now on page 39 of game engine ui 3 powerpoint.
private:

};

