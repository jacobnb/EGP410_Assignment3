#include <assert.h>
#include "StateMachine.h"

StateMachineState::~StateMachineState(){
	for (auto it = mTransitions.begin(); it != mTransitions.end(); ++it){
		if(it->second && it->second->getTargetStateID() >= 0){
			delete it->second;
			it->second = NULL;
		}
	}
}

void StateMachineState::addTransition( StateTransition* pTransition )
{
	mTransitions[ pTransition->getTransitionType()] = pTransition;
}

StateMachine::~StateMachine(){
	for (auto it = mStates.begin(); it != mStates.end(); ++it){
		delete it->second;
		it->second = NULL;
	}
}


void StateMachine::addState( StateMachineState* pState )
{
	mStates[ pState->getID() ] = pState;
}

void StateMachine::start()
{
	if( mInitialStateID != -1 )
	{
		transitionToState( mInitialStateID );
	}
}

void StateMachine::update()
{
	if( mpCurrentState == NULL )
	{
		start();
		assert( mpCurrentState != NULL );
	}

	StateTransition* pTransition = mpCurrentState->update();
	if( pTransition != NULL )
	{
		transitionToState( pTransition->getTargetStateID() );
	}
}

void StateMachine::transitionToState( const SM_idType& targetID )
{
	if( mpCurrentState != NULL )
	{
		mpCurrentState->onExit();
	}

	mpCurrentState = mStates[targetID];
	if( mpCurrentState != NULL )
	{
		mpCurrentState->onEntrance();
	}
}
