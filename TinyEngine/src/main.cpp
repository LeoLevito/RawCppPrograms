#include "Engine/TinyEngine.h"
#include "PlayerInput.h"
#include "Actor.h"




#define numActors 20 //macro, very powerful, need to learn more. could also do const int in this case.

//so, here's a DECLARATION
void DrawActor(Actor actor);

int main() {

	engInit("Tiny Engine", 800, 600); //name and size of window
	//Actor player = Actor(Vector(50.f, 50.f), Vector(32.f), COLOR_WHITE);
	//// Literals (any hard coded value)
	float speed{ 100.f }; //floating point literal, .f instead of just f in c#, otherwise it would just be an int before being automatically converted to float anyways.
	//const int numActors{ 10 };
	Actor actors[numActors];

	for (int i = 0; i < numActors; i++) {
		actors[i] = Actor(Vector(engRandom(800), engRandom(600)), Vector(16.f), COLOR_SALMON);
	}
	Actor* player = &actors[0];
	(*player) = Actor(Vector(400.f, 300.f), Vector(32.f), COLOR_WHITE); //utilizing constructor instead of setting position, size and color individually


	int myInt{ 10 };
	int myInt2{ 10 };
	int* myPointer{ &myInt}; //pointer is a type + address, stores an adress to something.
	*myPointer = 50;//de-reference of a pointer, i.e set myInt to 50
	myPointer = &myInt2;
	*myPointer = 100;//de-reference of a pointer, again. i.e set myInt2 to 100
	myPointer = nullptr;

	while (engBeginFrame()) {

		engSetDrawColor(COLOR_DARK_GRAY);
		engClearScreen();

		UpdateInput(player->position, speed); //player->position == (*player).position
		//DrawActor(player);

		//draw all actors

		for (int i = 0; i < numActors; i++) {
			actors[i].Draw();
		}

		if (engKeyPressed(Key::Escape)) {
			break;
		}
	}
}