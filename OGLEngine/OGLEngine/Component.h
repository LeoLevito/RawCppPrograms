#pragma once
#include <string>
class Component
{
public:
	std::string name;
	virtual void DrawComponentSpecificImGuiHierarchyAdjustables();
private:
};

