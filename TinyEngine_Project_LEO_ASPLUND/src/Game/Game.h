#pragma once
#include "Camera.h"
#include "TimerSystem.h"

#define MAX_ACTORS 100

// Forward-declare
class Actor;
enum class Collision_Channel;

class Game
{
public:
	Game();

	void update();
	void render();

	Actor* get_player() { return player; }
	Camera& get_camera() { return camera; }

	template<typename ActorType>
	ActorType* spawn_actor(Vector position)
	{
		for (int i = 0; i < MAX_ACTORS; ++i)
		{
			if (actors[i] == nullptr)
			{
				ActorType* new_actor = new ActorType(position);
				actors[i] = new_actor;

				return new_actor;
			}
		}

		return nullptr;
	}

	Actor* get_colliding_actor(Actor* other, Collision_Channel channel);

private:
	static constexpr float GRID_SIZE = 100.f;

	Actor* actors[MAX_ACTORS] = { nullptr };
	Actor* player = nullptr;

	Camera camera;
	Timer_System timers;
};

extern Game* game;
