#pragma once

#include "Trackable.h"
#include "PerformanceTracker.h"
#include <string>


//==Gets keyboard and mouse input then sends relevant messages==//
class InputSystem: public Trackable
{
public:
	InputSystem();
	~InputSystem();
	void update(); 
	void clearQueue();
	void updateMouseEvents();
private:
	int mXMouse, mYMouse;
	int mPrevX, mPrevY;
};
