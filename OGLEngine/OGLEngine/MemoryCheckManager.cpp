#include "MemoryCheckManager.h"
#include <mutex>
#include "MemoryMessage.h"
#include <windows.h>
#include <iostream>
#include "MeshManager.h"


std::mutex myMutex3; //oh my god I can't be doing this, I need to have a new mutex name for every new mutex I make. Not convenient.

MemoryCheckManager& MemoryCheckManager::Get()
{
	static MemoryCheckManager instance;
	return instance;
	// TODO: insert return statement here
}

void MemoryCheckManager::QueueMessage(Message* message)
{
	//mutex, since we modify the queuedmessages here
	std::lock_guard<std::mutex> guard(myMutex3); //wrap into Mutex class.
	queuedMessages.push_back(message);
	//mutex is released when out of scope, lock_guard does this automatically for us.
}

void MemoryCheckManager::ProcessMessages()
{
	//mutex since we modify the queuedmessages here
	std::lock_guard<std::mutex> guard(myMutex3); //wrap into Mutex class.
	while (queuedMessages.size() >= 1)
	{
		Message* message = queuedMessages.front();
		ProcessMessage(message);
		queuedMessages.erase(std::remove(queuedMessages.begin(), queuedMessages.end(), message));
		delete message;
	}
	//mutex is released when out of scope, lock_guard does this automatically for us.
}

void MemoryCheckManager::ProcessMessage(Message* message)
{
	std::string& msg = message->msg;
	switch (message->type)
	{
	case MessageType::MemoryMessage:
	{
		MemoryMessage& memoryMessage = dynamic_cast<MemoryMessage&>(*message); //is this bad practice for messaging? RE:(MeshComponent), apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times. Plus this cast only happens when processing messages, so not too frequently.
		MemoryMessage* newMessage = new MemoryMessage();
		newMessage->isAvailableMemoryOK = CheckEnoughMemoryAvailable();
		newMessage->isFileSizeOK = CheckFileSizeOK(memoryMessage.directoryEntry);
		MeshManager::Get().QueueMessage(newMessage);
		break;
	}
	default:
		break;
	}
}

void MemoryCheckManager::Process()
{
	while (shouldRun)
	{
		ProcessMessages();
	}
}

bool MemoryCheckManager::CheckEnoughMemoryAvailable()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	if ((statex.ullAvailPhys / (1024 * 1024)) < 500) //could improve this by checking the size of the requested .obj file, if it's bigger than say 100MB return this error.
	{
		std::cout << "There is " << statex.ullAvailPhys / (1024 * 1024) << " MB of physical memory available." << std::endl;
		std::cout << "There is less than 500 MB of available Physical Memory, cancelling mesh allocation." << std::endl;
		return false;
	}

	if ((statex.ullAvailVirtual / (1024 * 1024)) < 100)
	{
		std::cout << "There is " << statex.ullAvailVirtual / (1024 * 1024) << " MB of virtual memory available." << std::endl;
		std::cout << "There is less than 100 MB of available Virtual Memory, cancelling mesh allocation." << std::endl;
		return false;
	}

	return true;
}

bool MemoryCheckManager::CheckFileSizeOK(std::filesystem::directory_entry directoryEntry)
{
	float objsize = directoryEntry.file_size();
	float objsizeInMB = (objsize / 1024.f) / 1024.f;
	std::cout << "file size of .obj: " << objsizeInMB << " MB" << std::endl;

	if (objsizeInMB > 100.f) //could make a comparison here with available memory instead. With a good margin as well since sometimes memory taken up by a mesh is larger than its file size.
	{
		return false; //too big file size, not OK.
	}
	else
	{
		return true; //OK file size
	}
}

void MemoryCheckManager::PrintMemoryStatus()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	std::cout << "There is " << statex.ullAvailPhys / (1024 * 1024) << " MB of physical memory available." << std::endl;
	std::cout << "There is " << statex.ullAvailVirtual / (1024 * 1024) << " MB of virtual memory available." << std::endl;
}
