#include "TimerSystem.h"
#include "Engine/TinyEngine.h"

void Timer_System::add_timer(float duration, Callback callback, bool looping)
{
	Timer timer;
	timer.looping = looping;
	timer.start_time = engCurrentTime();
	timer.duration = duration;
	timer.callback = callback;

	timers.push_back(timer);
}

void Timer_System::update()
{
	for (int i = 0; i < timers.size(); ++i)
	{
		if (engTimePassedSince(timers[i].start_time) >= timers[i].duration)
		{
			timers[i].callback();
			if (timers[i].looping)
			{
				timers[i].start_time += timers[i].duration;
			}
			else
			{
				timers.erase(timers.begin() + i);
				i--;
			}
		}
	}
}
