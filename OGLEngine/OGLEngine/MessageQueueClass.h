#pragma once
#include "Message.h"
#include <vector>
class MessageQueueClass //make singleton? Like a MessageManager??? 
{
public:
	void QueueMessage(Message* message); //in case of a singleton, wouldn't you want/need to specify what class the message is going to?
private:
	void ProcessMessages();
	void ProcessMessage(Message* message);
	//...
	std::vector <Message*> messages;
};

