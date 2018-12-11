#include "WanderState.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

void WanderState::onEntrance(){
	//If you are the player make sure you are in AI Mode
	if(mPlayer){
		if(gpGame->getAIFight()){
			Unit* owner = gpGame->getUnitManager()->getUnit(mOwnerID);
			owner->setSteering(Steering::WANDERPATH, ZERO_VECTOR2D);
		}
	}
	else {
		Unit* owner = gpGame->getUnitManager()->getUnit(mOwnerID);
		owner->setSteering(Steering::WANDERPATH, ZERO_VECTOR2D);
	}
}

void WanderState::onExit(){

}

StateTransition* WanderState::update(){
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	if(!mPlayer){
		//if you see an enemy engage them
		Unit* player = gpGame->getUnitManager()->getPlayerUnit();
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
		//if you see a player but they are powered up run away
	}
	else {
		//if you are a player then run away from enemy unless powered up
		if(pOwner->poweredUp()){
			std::vector<Unit*> units = gpGame->getUnitManager()->getAllUnits();
			for(int i = 1; i < units.size(); i++){
				Unit* enemy = units[i];
				Vector2D diff = enemy->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

				auto distance = diff.getLength();
				if(distance < mFollowRange){
					std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( TOWARDS_TRANSITION );
					if( iter != mTransitions.end() ) //found?
					{
						StateTransition* pTransition = iter->second;
						return pTransition;
					}
				}
			}
		}
		else {
			//Run away
			std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( RUNAWAY_TRANSITION );
			if( iter != mTransitions.end() )//found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}
	}
	if(mPlayer && gpGame->getAIFight() && !currentlyFighting){
		Unit* owner = gpGame->getUnitManager()->getUnit(mOwnerID);
		owner->setSteering(Steering::WANDERPATH, ZERO_VECTOR2D);
		currentlyFighting = true;
	}
	if(mPlayer && !gpGame->getAIFight()){
		currentlyFighting = false;
	}
	//if you see an item move to it

	//else just return null
	return NULL;
}