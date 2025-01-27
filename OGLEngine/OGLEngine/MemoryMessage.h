#pragma once
#include "Message.h"
#include <filesystem>

class MemoryMessage : public Message
{
public:
	MemoryMessage() : Message(MessageType::MemoryMessage, "hello") {}
	std::filesystem::directory_entry directoryEntry;
	~MemoryMessage();

	bool isFileSizeOK;
	bool isAvailableMemoryOK;
private:
};

