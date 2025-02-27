#pragma once
#include <string>

enum class MessageType {
	String,
	Mouse,
	FloatMessage,
	Raycast,
	MeshMessage,
	ObjectMessage,
	MemoryMessage,
	LevelMessage
};

class Message
{
public:
	Message(MessageType type, std::string msg);
	virtual ~Message();
	MessageType type;
	std::string msg;
private:

};

