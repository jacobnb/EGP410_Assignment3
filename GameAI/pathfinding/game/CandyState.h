#pragma once

#include "StateMachine.h"

class CandyState : public StateMachineState
{
public:
	CandyState( const SM_idType& id , bool player, int ownerID, int followRange = 250):StateMachineState(id), mPlayer(player), mOwnerID(ownerID), mFollowRange(followRange){};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

private:
	bool mPlayer;
	int mOwnerID;
	int mFollowRange;
};
