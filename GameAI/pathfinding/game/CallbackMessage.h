#pragma once

#include "GameMessage.h"
class Unit;
//==Message to run a callback==//
class CallbackMessage :public GameMessage
{
public:
	CallbackMessage(void (Unit::*functionPointer)(void));
	~CallbackMessage();

	void process();

private:
	void (Unit::*callBack)(void);
};