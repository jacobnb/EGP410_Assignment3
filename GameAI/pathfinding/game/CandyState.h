#pragma once

#include "StateMachine.h"

class CandyState : public StateMachineState
{
public:
	CandyState( const SM_idType& id ):StateMachineState(id){};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();
};
