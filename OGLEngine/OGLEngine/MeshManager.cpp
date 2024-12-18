#include "MeshManager.h"
#include <iostream>
#include <windows.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "MeshMessage.h"
#include <mutex>

std::mutex myMutex;

MeshManager::MeshManager()
{
	objreader = new ObjReader;
}

MeshManager::~MeshManager()
{
	delete objreader;
}

MeshManager& MeshManager::Get()
{
	static MeshManager instance; //so this will be instanciated once and then on subsequent calls of this Get() function it will just be referenced! See https://youtu.be/PPup1yeU45I?t=888 for more info.
	return instance;
}

Mesh* MeshManager::LoadMesh(const std::string& filename)
{
	for (size_t i = 0; i < CachedMeshes.size(); i++) 
	{
		if (CachedMeshes[i] == filename) { //since ImGui entry doesn't require proper capitalization, there can be situations where we load the same mesh multiple times when we don't need to. This would be fixed by having a dropdown to choose meshes from instead of a string entry.
			lastAccessedMesh = meshes[i];
			return lastAccessedMesh;
		}
	}

	PrintMemoryStatus();
	Mesh* mesh = new Mesh(objreader, filename);
	PrintMemoryStatus();
	//18 dec 2024: if check for checking maximum memory, return error if requested memory allocation is too large. Could also use messages for this.
	//18 dec 2024: Send message back to some manager telling us that the mesh has been loaded, to fulfill G requirement - Emil.

	meshes.push_back(mesh);
	CachedMeshes.push_back(filename);
	lastAccessedMesh = mesh;
	return lastAccessedMesh;
}

void MeshManager::QueueMessage(Message* message)
{
	//mutex, since we modify the queuedmessages here
	std::lock_guard<std::mutex> guard(myMutex); //wrap into Mutex class.
	queuedMessages.push_back(message);
	//mutex is released when out of scope, lock_guard does this automatically for us.
}

void MeshManager::ProcessMessages()
{
	//mutex since we modify the queuedmessages here
	std::lock_guard<std::mutex> guard(myMutex); //wrap into Mutex class.
	while (queuedMessages.size() >= 1)
	{
		Message* message = queuedMessages.front();
		ProcessMessage(message);
		queuedMessages.erase(std::remove(queuedMessages.begin(), queuedMessages.end(), message));
		delete message;
	}
	//mutex is released when out of scope, lock_guard does this automatically for us.
}

void MeshManager::ProcessMessage(Message* message)
{
	std::string& msg = message->msg;
	switch (message->type)
	{
	case MessageType::String:
		break;
	case MessageType::MeshMessage:
		MeshMessage& meshmessage = dynamic_cast<MeshMessage&>(*message); //is this bad practice for messaging? RE:(MeshComponent), apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times. Plus this cast only happens when processing messages, so not too frequently.
		currentlyLoadingMesh = true;
		LoadMesh(meshmessage.meshToLoad);
		currentlyLoadingMesh = false;
		break;
	}
}

void MeshManager::Process()
{
	while (shouldRun)
	{
		ProcessMessages();
	}
}

void MeshManager::PrintMemoryStatus()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	std::cout << "There is " << statex.ullAvailPhys / (1024 * 1024) << " MB of physical memory available." << std::endl;
	std::cout << "There is " << statex.ullAvailVirtual / (1024 * 1024) << " MB of virtual memory available." << std::endl;
}
