#pragma once
#include <vector>
#include <functional>

class Timer_System
{
public:
	using Callback = std::function<void()>;

	void add_timer(float duration, Callback callback, bool looping = false);
	void update();

private:
	struct Timer
	{
		bool looping = false;
		float start_time = 0.f;
		float duration = 0.f;
		Callback callback;
	};

	std::vector<Timer> timers;
};
