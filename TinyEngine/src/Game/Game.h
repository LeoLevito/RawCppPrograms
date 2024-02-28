#pragma once
//#include "Actor.h"
#include "Camera.h"

// macro
#define maxActors 100 //macro, very powerful, need to learn more. could also do const int in this case.

// Forward-declare actor class. 
// Can use pointers to point to the class but you can not use the class directly now. 
// For that you use the #include "Actor.h". 
// Good practice to forward declare whenever possible in .h files, since those #include's are copied recursively. 
// #Include's are kinda expensive because of its copy paste nature in compile time.
class Actor;

//forward-declare enum
enum class CollisionChannel;

class Game {
public:
	Game();
	void Update();
	void Render();

	Actor* GetPlayer() { return player; }//Getter function
	Camera& GetCamera() { return camera; }//returns reference to have the actual camera instead of copying the camera, if i understand correctly.

	void SpawnActor(Actor* actor);

	Actor* GetCollidingActor(Actor* other, CollisionChannel channel);

private:
	static constexpr float SPAWNINTERVAL{ 0.5f };
	static constexpr float GRIDSIZE{ 100.f };

	Actor* actors[maxActors]{ nullptr };
	Actor* player{ nullptr };

	Camera camera;

	// Literals (any hard coded value)
	float speed{ 100.f }; //floating point literal, .f instead of just f in c#, otherwise it would just be an int before being automatically converted to float anyways.
	float lastSpawnTime{ 0 };
};


extern Game* game; //somewhere in this program there's a global variable called game. Declare global variables this way.

