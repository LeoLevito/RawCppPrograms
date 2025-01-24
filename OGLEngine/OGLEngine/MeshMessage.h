#pragma once
#include "Message.h"
#include "MeshComponent.h"
#include <filesystem>

class Mesh;
class MeshComponent;


class MeshMessage : public Message
{
public:
	MeshMessage() : Message(MessageType::MeshMessage, "hello") {}
	Mesh* meshToPass;
	std::string meshToLoad;
	std::filesystem::directory_entry directoryEntry;
	~MeshMessage();
private:
};

