#pragma once
#include "Math/Vector.h"
class Camera{
public:
	void Update();
	static constexpr float CAMERASPEED = 8.5f;
	Vector worldToScreen(Vector vec);
	Vector screenToWorld(Vector vec);
	Vector position;
};

