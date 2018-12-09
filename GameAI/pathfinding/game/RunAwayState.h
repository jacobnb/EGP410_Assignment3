#pragma once

#include "StateMachine.h"

class RunAwayState : public StateMachineState
{
public:
	RunAwayState( const SM_idType& id ):StateMachineState(id){};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();
};