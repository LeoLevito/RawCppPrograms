#include "Camera.h"
#include "Game.h"
#include "Actor.h" //if this would be in the .h file, we would have a circular dependency, but it's fine here in the .cpp file.
#include "Config.h"
void Camera::Update(){
	if (game->GetPlayer() != nullptr) {
		Vector playerPosition = game->GetPlayer()->position;
		Vector mousePosition = screenToWorld(Vector(engMouseX(), engMouseY()));

		Vector targetPosition = Vector::lerp(playerPosition, mousePosition, 0.3f);

		//Vector camOffset = mousePosition - playerPosition;

		position = Vector::lerp(position, targetPosition, CAMERASPEED * engDeltaTime());
	}
}

Vector Camera::worldToScreen(Vector vec){
	using namespace Config;
	return vec - position + Vector(WINDOWWIDTH / 2, WINDOWHEIGHT / 2);
}

Vector Camera::screenToWorld(Vector vec){
	using namespace Config;
	return vec + position - Vector(WINDOWWIDTH / 2, WINDOWHEIGHT / 2);
}
