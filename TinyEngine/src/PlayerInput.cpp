#include "PlayerInput.h"
#include "Engine/TinyEngine.h"


//so, here's a DEFINITION, can also have this function before main function, since c++ compiles from top to bottom.
//pass-by-value parameters (e.g. just regular floats), when you call the funtion, copy the parameters to the function, so they are copies but not the original variables.
//pass-by-reference values (e.g. float&), uses the original value and does not create a copy.
void UpdateInput(Vector& position, float& speed) {
	Vector input;
	if (engKeyDown(Key::W)) {
		input.y -= 1.f;
	}
	if (engKeyDown(Key::S)) {
		input.y += 1.f;
	}
	if (engKeyDown(Key::D)) {
		input.x += 1.f;
	}
	if (engKeyDown(Key::A)) {
		input.x -= 1.f;
	}

	input.Normalize();
	position += input * speed * engDeltaTime(); //this was made possible using the operator overload defined in the Vector.h struct

	if (engKeyPressed(Key::E)) {
		speed += 50.f;
	}
	if (engKeyPressed(Key::Q)) {
		speed -= 50.f;
	}
}
//int addIntegers(int a, int b) {
//	return a + b;
//}
//int multiplyIntegers(int a, int b) {
//	return a * b;
//}