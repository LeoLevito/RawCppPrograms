#pragma once
#include "Math/Vector.h"

class Camera
{
public:
	void update();

	static constexpr float CAMERA_SPEED = 8.5f;

	Vector world_to_screen(Vector vec);
	Vector screen_to_world(Vector vec);

	Vector position;
};
