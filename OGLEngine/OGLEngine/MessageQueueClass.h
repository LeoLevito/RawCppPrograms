#pragma once
#include "Message.h"
#include <vector>
class MessageQueueClass //make singleton? Like a MessageManager??? 
{
public:
	void QueueMessage(Message* message); //in case of a singleton, wouldn't you want/need to specify what class the message is going to?
										 //maybe the G requirement is that you have a message queue in one of your managers? The lecture MessageQueueClass may not be applicable to the G requirement. Maybe I should ask Emil that.
private:
	void ProcessMessages();
	void ProcessMessage(Message* message);
	//...
	std::vector <Message*> messages;
};

