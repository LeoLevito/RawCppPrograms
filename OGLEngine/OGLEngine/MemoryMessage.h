#pragma once
#include "Message.h"
#include <filesystem>

enum class MemoryMessageType {
	RequestFileSize,
	RequestAvailableMemory
};

class MemoryMessage : public Message
{
public:
	MemoryMessage() : Message(MessageType::MemoryMessage, "hello") {}
	MemoryMessageType myMessageType;
	std::filesystem::directory_entry directoryEntry;
	~MemoryMessage();

	bool isFileSizeOK;
	bool isAvailableMemoryOK;
private:
};

