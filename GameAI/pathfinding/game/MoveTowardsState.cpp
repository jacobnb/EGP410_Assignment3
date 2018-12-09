#include "MoveTowardsState.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

void MoveTowardsState::onEntrance(){
	if(!mPlayer){
		Unit* player = gpGame->getUnitManager()->getPlayerUnit();
		Unit* owner = gpGame->getUnitManager()->getUnit(mOwnerID);
		owner->setSteering(Steering::PATHFINDFOLLOW, ZERO_VECTOR2D, player->GetID());
	}
}

void MoveTowardsState::onExit(){

}

StateTransition* MoveTowardsState::update(){
	//if you are x distance away from an enemy go wander again
	if(!mPlayer){
		Unit* player = gpGame->getUnitManager()->getPlayerUnit();
		Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
		Vector2D diff = player->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

		auto distance = diff.getLength();
		if(distance > mFollowRange){
			std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( WANDER_TRANSITION );
			if( iter != mTransitions.end() )//found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}
	}
	//else just return null
	return NULL;
}