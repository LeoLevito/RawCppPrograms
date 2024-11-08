#include "EngineTime.h"

void EngineTime::UpdateDeltaTime(const float& deltaTime)
{
	myDeltaTime = deltaTime;
}

const float& EngineTime::DeltaTime()
{
	return myDeltaTime;
}
