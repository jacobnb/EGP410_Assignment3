#pragma once

#include "StateMachine.h"

class RunAwayState : public StateMachineState
{
public:
	RunAwayState( const SM_idType& id , bool player, int ownerID, int followRange = 250):StateMachineState(id), mPlayer(player), mOwnerID(ownerID), mFollowRange(followRange){};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();
	void setUpPlayerAI();

private:
	bool mPlayer;
	int mOwnerID;
	int mFollowRange;
	bool currentlyFighting = false;
};