#include "WanderState.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

void WanderState::onEntrance(){
	Unit* owner = gpGame->getUnitManager()->getUnit(mOwnerID);
	owner->setSteering(Steering::WANDERPATH, ZERO_VECTOR2D);
}

void WanderState::onExit(){

}

StateTransition* WanderState::update(){
	//if you see an enemy engage them
	if(!mPlayer){
		Unit* player = gpGame->getUnitManager()->getPlayerUnit();
		Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
		Vector2D diff = player->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

		auto distance = diff.getLength();
		if(distance < mFollowRange){
			std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( TOWARDS_TRANSITION );
			if( iter != mTransitions.end() )//found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}
	}
	//if you see an item move to it

	//if you see a player but they are powered up run away

	//else just return null
	return NULL;
}