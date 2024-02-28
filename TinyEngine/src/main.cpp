#include "Engine/TinyEngine.h"
#include "Actor.h"
#include "Game/Game.h"
#include "Config.h"


int hello3{ 0 }; //Static memory allocation, since we're outside any function.
int main() {

	engInit("Tiny Engine", Config::WINDOWWIDTH, Config::WINDOWHEIGHT); //name and size of window
	game = new Game();

	Actor* actors[maxActors]{nullptr}; //array of pointers, need to initialize array now. Illegal to reference nullpointers
	actors[0] = new Actor(Vector(400.f, 300.f), Vector(32.f), COLOR_WHITE); //Heap allocated actor.
	/*actors[1] = new Actor(Vector(100.f, 100.f), Vector(20.f), COLOR_GREEN);
	actors[2] = new Actor(Vector(600.f, 400.f), Vector(40.f), COLOR_CYAN);*/ //if doing actors[1] again, it allocates on the heap again, overwriting the pointer and thus we are unable to de-allocate that specific pointer ever again. Memory leak, don't do this mistake.
	int actorCount{ 0 };


	Actor* player = actors[0];
	(*player) = Actor(Vector(400.f, 300.f), Vector(32.f), COLOR_WHITE); //utilizing constructor instead of setting position, size and color individually

	int* hello = new int(); // Heap allocation. (right?), yes new int is allocating on heap, 
	int hello2{ 0 }; // Stack allocation, since we're inside a function. Stack overflow happens when a function attempts to allocate too much memory. Careful with self-recursive functions.
	*hello = 20; // dynamic heap allocation.

	delete hello; //de-allocate heap allocated data. i.e. free up memory. very important, if you're doing 'new int()' multiple times at runtime, you need to use delete operator to avoid memory leak.


	int myInt{ 10 };
	int myInt2{ 10 };
	int* myPointer{ &myInt }; //pointer is a type + address, stores an adress to something.
	*myPointer = 50;//de-reference of a pointer, i.e set myInt to 50
	myPointer = &myInt2;
	*myPointer = 100;//de-reference of a pointer, again. i.e set myInt2 to 100
	myPointer = nullptr;

	while (engBeginFrame()) {

		engSetDrawColor(COLOR_DARK_GRAY);
		engClearScreen();

		if (engKeyPressed(Key::I)) {
			if (actorCount < maxActors - 1) {
				actors[actorCount + 1] = new Actor(Vector(engRandom(0, 800), engRandom(0, 800)), Vector(engRandom(16, 32)), COLOR_RED);
				actorCount++;
			}
		}
		if (engKeyPressed(Key::U)) {
			if (actorCount > 0) {
				actorCount--;
				delete actors[actorCount + 1];		//most common crash in c++ is trying to reference de-allocated memory
				actors[actorCount + 1] = nullptr; // delete doesn't automatically nullptr a pointer. Need to do that manually.
			}
		}

		
		//DrawActor(player);



		if (engKeyPressed(Key::Escape)) {
			return 0;
		}













		game->Update();
		game->Render();
	}

	delete game;
}