#pragma once
#include <vector>	//part of the c++ standard library
#include <functional>	//part of the c++ standard library

class TimerSystem{
public:
	using Callback = std::function<void()>; //see main.cpp for more info on Typedef and Using.
	void addTimer(float duration, Callback callback, bool looping = false); //looping = false is a default argument. Unly used in the header file, not in the cpp file.
	void Update();
private:
	struct Timer { //defining struct inside of class. Neat for organization.
		bool looping = false;
		float startTime{ 0.f };
		float duration{ 0.f };
		Callback callback;
	};

	std::vector<Timer> timers; //dynamic array in Standard library.
};

