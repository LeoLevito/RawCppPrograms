#include "MeshManager.h"
#include <iostream>
#include <windows.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "MeshMessage.h"
#include <mutex>
#include "MemoryMessage.h"
#include "MemoryCheckManager.h"

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

	//will need to move this to a different function so I can stagger it after message processing. Shit man this will be hard. I need to refactor in the mesh component as well.
	if (IsAvailableMemoryOK == true) //now I just need to refactor this with a new MemoryCheckManager class.
	{
		if (IsObjSizeOK == true)
		{
			Mesh* mesh = new Mesh(objreader, filename);
			meshes.push_back(mesh);
			CachedMeshes.push_back(filename);
			lastAccessedMesh = mesh;
			return lastAccessedMesh;
		}
		else
		{
			allowObjSizeErrorPopup = true;
		}
	}
	else
	{
		allowLowMemoryErrorPopup = true;
	}

	//18 dec 2024: if check for checking maximum memory, return error if requested memory allocation is too large. Could also use messages for this. 
	// So like, maybe, a memory checker manager where I send a request message and I receive a result message?!?!?!?!?! I think I could do that. That would kill two birds with one stone! As long as I thread that class as well. I would get an extra message subclass, I would communicate between two managers, and it would be threaded AND only communicated through messages.
	//18 dec 2024: Send message back to some manager telling us that the mesh has been loaded, to fulfill G requirement - Emil.
}

void MeshManager::RequestMemoryCheck(std::filesystem::directory_entry directoryEntry)
{
	MemoryMessage* newMessage = new MemoryMessage();
	newMessage->directoryEntry = directoryEntry;
	MemoryCheckManager::Get().QueueMessage(newMessage);
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
	{
		MeshMessage& meshmessage = dynamic_cast<MeshMessage&>(*message); //is this bad practice for messaging? RE:(MeshComponent), apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times. Plus this cast only happens when processing messages, so not too frequently.
		currentDirectoryEntry = meshmessage.directoryEntry;
		currentMeshName = meshmessage.meshToLoad;
		currentlyLoadingMesh = true;
		RequestMemoryCheck(currentDirectoryEntry);
		break;
	}
	case MessageType::MemoryMessage:
	{
		MemoryMessage& memoryMessage = dynamic_cast<MemoryMessage&>(*message); //is this bad practice for messaging? RE:(MeshComponent), apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times. Plus this cast only happens when processing messages, so not too frequently.
		IsAvailableMemoryOK = memoryMessage.isAvailableMemoryOK;
		IsObjSizeOK = memoryMessage.isFileSizeOK;
		LoadMesh(currentMeshName); //Using data 'meshToLoad' from the message casted to meshmessage. We wouldn't need to cast if we were calling a class function that doesn't need specific data from a message.
		currentlyLoadingMesh = false; //this needs to be moved;
		break;
	}
	default:
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

void MeshManager::ErrorGUI()
{
	if (allowLowMemoryErrorPopup)
	{
		ImGui::OpenPopup("NotEnoughMemoryPopupModal");
		allowLowMemoryErrorPopup = false;
	}
	if (ImGui::BeginPopupModal("NotEnoughMemoryPopupModal"))
	{
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);
		int phys = statex.ullAvailPhys / (1024 * 1024);
		int virt = statex.ullAvailVirtual / (1024 * 1024);

		ImGui::Text("Error message: Not enough memory available to safely load meshes. ( <500 MB)");
		ImGui::Text("Try upgrading your computer :)");
		ImGui::Separator();
		ImGui::Text("Available Physical Memory in MB: %i", phys);
		ImGui::Text("Available Virtual Memory in MB: %i", virt);

		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (allowObjSizeErrorPopup)
	{
		ImGui::OpenPopup("ObjSizeTooBigPopupModal");
		allowObjSizeErrorPopup = false;
	}
	if (ImGui::BeginPopupModal("ObjSizeTooBigPopupModal"))
	{
		ImGui::Text("Error message: Requested .obj file is too big to load safely. ( >100 MB)");
		ImGui::Text("Try decimating the mesh in your 3D program and then re-export it.");

		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
