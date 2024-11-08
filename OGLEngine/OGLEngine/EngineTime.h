#pragma once
class EngineTime
{
public:
	void UpdateDeltaTime(const float& deltaTime);
	const float& DeltaTime();
private:
	float myDeltaTime;
};

