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
		Unit* player = gpGame->getUnitManager()->getPlayerUnit();
		Vector2D diff = player->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

		auto distance = diff.getLength();
		//if you see a player but they are powered up run away
		if(player->poweredUp()){
			std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( RUNAWAY_TRANSITION );
			if( iter != mTransitions.end() ) //found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}

		//if you see an enemy engage them
		if(distance < mFollowRange){
			std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( TOWARDS_TRANSITION );
			if( iter != mTransitions.end() ) //found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}

		//if you see an enemy candy move to it
		std::vector<Unit*> units = gpGame->getUnitManager()->getAllUnits();
		for(int i = 0; i < units.size(); i++){
			if(units[i]->getType() == Unit::TYPE::ENEMY_FOOD){
				Unit* food = units[i];
				Vector2D diff = food->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

				auto distance = diff.getLength();
				if(distance < mFollowRange){
					std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( CANDY_TRANSITION );
					if( iter != mTransitions.end() ) //found?
					{
						StateTransition* pTransition = iter->second;
						return pTransition;
					}
				}
			}
		}
	}
	else {
		//if you are a player then run away from enemy unless powered up
		std::vector<Unit*> units = gpGame->getUnitManager()->getAllUnits();
		for(int i = 0; i < units.size(); i++){
			if(units[i]->getType() == Unit::TYPE::ENEMY){
				Unit* enemy = units[i];
				Vector2D diff = enemy->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

				auto distance = diff.getLength();
				if(distance < mFollowRange){
					if(pOwner->poweredUp()){
						std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( TOWARDS_TRANSITION );
						if( iter != mTransitions.end() ) //found?
						{
							StateTransition* pTransition = iter->second;
							return pTransition;
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
			}
			//Grab some candy if you see it
			if(units[i]->getType() == Unit::TYPE::MIGHTY_CANDY){
				Unit* food = units[i];
				Vector2D diff = food->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

				auto distance = diff.getLength();
				if(distance < mFollowRange){
					std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( CANDY_TRANSITION );
					if( iter != mTransitions.end() ) //found?
					{
						StateTransition* pTransition = iter->second;
						return pTransition;
					}
				}
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

	//else just return null
	return NULL;
}