#include "RunAwayState.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

void RunAwayState::onEntrance(){
	//If you are the player make sure you are in AI Mode
	if(!mPlayer){
		if(gpGame->getAIFight()){
			currentlyFighting = true;
			setUpPlayerAI();
		}
	}
	else {
		Unit* owner = gpGame->getUnitManager()->getUnit(mOwnerID);
		owner->setSteering(Steering::RUNAWAYPATHFINDING, ZERO_VECTOR2D, gpGame->getUnitManager()->getPlayerUnit()->GetID());
	}
}

void RunAwayState::onExit(){

}

StateTransition* RunAwayState::update(){
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	std::vector<Unit*> units = gpGame->getUnitManager()->getAllUnits();
	bool candyMove = false;
	bool runAway = false;
	if(!mPlayer){
		//if the player is no longer powered up go back to chasing the player
		Unit* player = gpGame->getUnitManager()->getPlayerUnit();
		if(!player->poweredUp()){
			std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( TOWARDS_TRANSITION );
			if( iter != mTransitions.end() ) //found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}
		//if the player is no longer in position, go back to wandering around
		Vector2D diff = player->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

		auto distance = diff.getLength();
		if(distance < mFollowRange){
			std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( WANDER_TRANSITION );
			if( iter != mTransitions.end() )//found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}
	}
	else {
		//if the player is powered up go to back to chasing the enemy
		if(pOwner->poweredUp()){
			std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( TOWARDS_TRANSITION );
			if( iter != mTransitions.end() ) //found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}
	}

	if(mPlayer && gpGame->getAIFight() && !currentlyFighting){
		setUpPlayerAI();
		currentlyFighting = true;
	}
	if(mPlayer && !gpGame->getAIFight()){
		currentlyFighting = false;
	}
	if(pOwner->isFinished && mPlayer){
		pOwner->isFinished = false;
		setUpPlayerAI();
		pOwner->isFinished = false;
	}
	if(pOwner->isFinished && !mPlayer){
		pOwner->isFinished = false;
		setUpEnemyAI();
		pOwner->isFinished = false;
	}
	if(mPlayer){
		if(pOwner->poweredUp()){
			std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( TOWARDS_TRANSITION );
			if( iter != mTransitions.end() ) //found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}
		//if the enemy is no longer in position, go back to wandering around
		for(int i = 0; i < units.size(); i++){
			Unit* food = units[i];
			Vector2D diff = food->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

			auto distance = diff.getLength();
			if(distance < mFollowRange && food->getType() == Unit::MIGHTY_CANDY){
				candyMove = true;
			}
			if(distance < mFollowRange && food->getType() == Unit::ENEMY){
				runAway = true;
			}
		}
		if(candyMove){
			std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( CANDY_TRANSITION );
			if( iter != mTransitions.end() ) //found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}
		if(runAway){
			return NULL;
		}
		std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( WANDER_TRANSITION );
		if( iter != mTransitions.end() ) //found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}

	//else just return null
	return NULL;
}

void RunAwayState::setUpPlayerAI(){
	std::vector<Unit*> units = gpGame->getUnitManager()->getAllUnits();
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	float maxDist = 5000;
	Unit* maxEnemy;
	for(int i = 0; i < units.size(); i++){
		if(units[i]->getType() == Unit::TYPE::ENEMY){
			Unit* enemy = units[i];
			Vector2D diff = enemy->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

			auto distance = diff.getLength();
			if(distance < maxDist){
				maxDist = distance;
				maxEnemy = enemy;
			}
		}
	}
	pOwner->setSteering(Steering::RUNAWAYPATHFINDING, ZERO_VECTOR2D, maxEnemy->GetID());
}

void RunAwayState::setUpEnemyAI(){
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Unit* player = gpGame->getUnitManager()->getPlayerUnit();
	pOwner->setSteering(Steering::RUNAWAYPATHFINDING, ZERO_VECTOR2D, player->GetID());
}