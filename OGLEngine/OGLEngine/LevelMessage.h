#pragma once
#include "Message.h"

enum class LevelMessageType {
	Save,
	Load
};

class LevelMessage : public Message
{
public:

	LevelMessageType myMessageType;
	LevelMessage(LevelMessageType messagetype) : Message(MessageType::LevelMessage, "hello")
	{
		myMessageType = messagetype;
	}
	~LevelMessage();
private:
};

