#include "Engine/TinyEngine.h"




//DECLARATION
//DEFINITION

//so, here's a DECLARATION
void UpdateInput(float& x, float& y, float& speed);
int main() {
	char MyCharacter{ 'A' };

	//C-Strings! are null terminated (0 at the end of the array, if you need to write 0 as a character write '\0'). 
	// The program doesn't know how long the string is, so the null terminator tells the program the string has ended.
	char MyString[5]{ "Hola" };
	float x{ 0.f };
	float y{ 0.f };

	// Literals (any hard coded value)
	float speed{ 100.f }; //floating point literal, .f instead of just f in c#, otherwise it would just be an int before being automatically converted to float anyways.

	//references
	float& speedReference = speed; //& functions kind of like a link, they are essentially the same variable.

	engInit("Tiny Engine", 800, 600); //name and size of window

	while (engBeginFrame()) {

		engSetDrawColor(COLOR_DARK_GRAY);
		engClearScreen();

		engSetDrawColor(COLOR_SALMON);
		engFillRect(x, y, 32, 32);

		engDrawTextF(10, 10, "speed: %f", speed);

		UpdateInput(x, y , speed);
		if (engKeyPressed(Key::Escape)) {
			break;
		}
	}
}

//so, here's a DEFINITION, can also have this function before main function, since c++ compiles from top to bottom.
//pass-by-value parameters (e.g. just regular floats), when you call the funtion, copy the parameters to the function, so they are copies but not the original variables.
//pass-by-reference values (e.g. float&), uses the original value and does not create a copy.
void UpdateInput(float& x, float& y, float& speed) {
	if (engKeyDown(Key::W)) {
		y -= speed * engDeltaTime();
	}
	if (engKeyDown(Key::S)) {
		y += speed * engDeltaTime();
	}
	if (engKeyDown(Key::D)) {
		x += speed * engDeltaTime();
	}
	if (engKeyDown(Key::A)) {
		x -= speed * engDeltaTime();
	}
	if (engKeyPressed(Key::E)) {
		speed += 50.f;
	}
	if (engKeyPressed(Key::Q)) {
		speed -= 50.f;
	}
}