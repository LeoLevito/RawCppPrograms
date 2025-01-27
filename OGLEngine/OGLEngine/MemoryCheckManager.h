#pragma once
#include "Message.h"
#include <vector>
#include <filesystem>
class MemoryCheckManager
{
public:
	static MemoryCheckManager& Get();

	void QueueMessage(Message* message);
	void ProcessMessages();
	void ProcessMessage(Message* message); //you have to use pointer here since a subclass of Message can be used.
	std::vector <Message*> queuedMessages;
	void Process();
	bool shouldRun = true;

	bool CheckEnoughMemoryAvailable();
	bool CheckFileSizeOK(std::filesystem::directory_entry directoryEntry);

	void PrintMemoryStatus();
private:
};

