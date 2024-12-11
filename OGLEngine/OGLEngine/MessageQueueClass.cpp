#include "MessageQueueClass.h"

void MessageQueueClass::QueueMessage(Message* message)
{
	messages.push_back(message);
}

void MessageQueueClass::ProcessMessages()
{
	while (messages.size())
	{
		/*Message* message = messages.pop_back();
		ProcessMessage(message);
		delete message;*/
	}

	//https://gamedev.stackexchange.com/questions/7718/event-driven-communication-in-a-game-engine-yes-or-no
	//https://en.wikipedia.org/wiki/Event-driven_programming
	//https://en.wikipedia.org/wiki/Message_queue
	//https://en.wikipedia.org/wiki/Mediator_pattern
	//https://en.wikipedia.org/wiki/Observer_pattern
}