#pragma once

#include "Trackable.h"
#include "PerformanceTracker.h"
#include <string>



class InputSystem: public Trackable
{
public:
	InputSystem();
	~InputSystem();

	std::string getKeyState();
	int getMouseState();
	void update(); //used to fire mouse events
	void update(int numEvents); //fires numEvents keyboard events.
	void updateAll(); //fires keyboard events.
	void clearQueue();
	void stuffFromGame();
	void updateMouseEvents();
private:
	int mXMouse, mYMouse;
	//GameMessage
};
