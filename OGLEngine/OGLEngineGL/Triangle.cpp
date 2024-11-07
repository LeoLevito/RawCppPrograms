#include "Triangle.h"

static float vertices[] =
{
	//normalized device coordinates, think x and y axis of your screen/window, from -1 to 1 on either axis.
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

Triangle::Triangle() : Mesh(vertices, sizeof(vertices), nullptr, 0) { } //Ohhh, the triangle constructor runs its parent constructor with the vertices array as inputs. I think I've done something similar in the past in some unreal or c++ project.