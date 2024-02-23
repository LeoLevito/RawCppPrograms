#pragma once
//#include "Actor.h"

// macro
#define maxActors 20 //macro, very powerful, need to learn more. could also do const int in this case.

// Forward-declare actor class. 
// Can use pointers to point to the class but you can not use the class directly now. 
// For that you use the #include "Actor.h". 
// Good practice to forward declare whenever possible in .h files, since those #include's are copied recursively. 
// #Include's are kinda expensive because of its copy paste nature in compile time.
class Actor;

class Game{
public:
	Game();
	void Update();
	void Render();

	Actor* GetPlayer() { //Getter function
		return player; 
	}
private:
	Actor* actors[maxActors]{nullptr};
	Actor* player{ nullptr };

	// Literals (any hard coded value)
	float speed{ 100.f }; //floating point literal, .f instead of just f in c#, otherwise it would just be an int before being automatically converted to float anyways.
};

extern Game game; //somewhere in this program there's a global variable called game. Declare global variables this way.

