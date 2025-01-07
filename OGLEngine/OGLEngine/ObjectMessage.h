#pragma once
#include "Message.h"
#include "GameObject.h"
#include <string>

enum class ObjectMessageType {
	Create,
	Delete
};

class ObjectMessage : public Message
{
public:

	ObjectMessageType myMessageType;
	ObjectMessage(ObjectMessageType messagetype) : Message(MessageType::ObjectMessage, "hello")
	{
		myMessageType = messagetype;
	}
	~ObjectMessage();
	GameObject* gameObjectToDelete;
private:
};