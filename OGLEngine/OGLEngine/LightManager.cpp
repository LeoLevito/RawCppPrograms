#include "LightManager.h"

LightManager LightManager::Get()
{
	static LightManager instance;
	return instance;
}
