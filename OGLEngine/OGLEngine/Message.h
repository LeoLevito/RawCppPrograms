#pragma once
#include <string>

enum class MessageType {
	String,
	Mouse,
	FloatMessage,
	Raycast,
};

class Message
{
public:
	Message(MessageType);
	virtual ~Message();
	MessageType type;
	std::string msg;
private:

};

