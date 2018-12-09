#pragma once

#include "StateMachine.h"

class EngageState : public StateMachineState
{
public:
	EngageState( const SM_idType& id ):StateMachineState(id){};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();
};
