#pragma once
#include "Message.h"
#include "MeshComponent.h"

class Mesh;
class MeshComponent;


class MeshMessage : public Message
{
public:
	MeshMessage() : Message(MessageType::MeshMessage, "hello") {}
	Mesh* meshToPass;
	std::string meshToLoad;
	~MeshMessage();
private:
};

