#pragma once
#include "Graphics.h" 
class Engine
{

public:
	void Initialize();
	void EngineTick();
private:
	Graphics* graphics;
	Engine* engine;
};

