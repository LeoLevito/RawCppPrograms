#include "TimerSystem.h"
#include "Engine/TinyEngine.h"

void TimerSystem::addTimer(float duration, Callback callback){
	Timer timer;
	timer.startTime = engCurrentTime();
	timer.duration = duration;
	timer.callback = callback;

	timers.push_back(timer);

}

void TimerSystem::Update(){
	for (int i = 0; i < timers.size(); i++) {
		if (engTimePassedSince(timers[i].startTime) >= timers[i].duration) {
			timers[i].callback();
			timers.erase(timers.begin() + i);

			i--;
		}
	}
}
