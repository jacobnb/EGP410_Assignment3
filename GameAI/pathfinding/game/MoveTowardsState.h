#pragma once

#include "StateMachine.h"

class MoveTowardsState : public StateMachineState
{
public:
	MoveTowardsState( const SM_idType& id ):StateMachineState(id){};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();
};