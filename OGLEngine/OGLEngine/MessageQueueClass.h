#pragma once
#include "Message.h"
#include <vector>
class MessageQueueClass
{
public:
	void QueueMessage(Message* message);
private:
	void ProcessMessages();
	void ProcessMessage(Message* message);
	//...
	std::vector <Message*> messages;
};

