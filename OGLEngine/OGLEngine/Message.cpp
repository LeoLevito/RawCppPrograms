#include "Message.h"

Message::Message(MessageType type, std::string msg) : type(type), msg(msg)
{

}

Message::~Message()
{
	//do cleanup if any data was allocated on the heap.
}
