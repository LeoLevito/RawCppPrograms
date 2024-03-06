#pragma once
#include <vector>

class TimerSystem{
public:
	using Callback = void(*)(); //see main.cpp for more info on Typedef and Using.

	void addTimer(float duration, Callback callback);
	void Update();
private:
	struct Timer { //defining struct inside of class. Neat for organization.
		float startTime{ 0.f };
		float duration{ 0.f };
		Callback callback = nullptr;
	};

	std::vector<Timer> timers; //dynamic array in Standard library.
};

