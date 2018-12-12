#include "MoveTowardsState.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "GameApp.h"
#include "Grid.h"
#include "GridGraph.h"
#include "GridPathfinder.h"
#include "Path.h"

void MoveTowardsState::onEntrance(){
	Unit* owner = gpGame->getUnitManager()->getUnit(mOwnerID);
	if(!mPlayer){
		Unit* player = gpGame->getUnitManager()->getPlayerUnit();
		owner->isFinished = true;
		//owner->setSteering(Steering::PATHFINDFOLLOW, ZERO_VECTOR2D, player->GetID());
	}
}

void MoveTowardsState::onExit(){

}

StateTransition* MoveTowardsState::update(){
	//if you are x distance away from an enemy go wander again
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	if(!mPlayer){
		Unit* player = gpGame->getUnitManager()->getPlayerUnit();
		if(player->poweredUp()){
			std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( RUNAWAY_TRANSITION );
			if( iter != mTransitions.end() )//found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}
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
		else if(pOwner->isFinished){
			pOwner->isFinished = false;
			PositionData data = pOwner->getPositionComponent()->getData();
			PositionData targetData = gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getData();
			GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
			GridPathfinder* pPathfinder = pGame->getPathfinder();
			GridGraph* pGridGraph = pGame->getGridGraph();
			Grid* pGrid = pGame->getGrid();

			int fromIndex = pGrid->getSquareIndexFromPixelXY(data.pos.getX(), data.pos.getY());
			int toIndex = pGrid->getSquareIndexFromPixelXY(targetData.pos.getX(), targetData.pos.getY());

			Node* pFromNode = pGridGraph->getNode(fromIndex);
			Node* pToNode = pGridGraph->getNode(toIndex);
			Path* pPath = pGame->getPathfinder()->findPath(pToNode, pFromNode);
			std::vector<Vector2D> nodePositions;
			if(!pPath){
				return NULL;
			}
			for (int c = 0; c < pPath->getNumNodes(); c++){
				nodePositions.push_back(pGrid->getULCornerOfSquare(pPath->peekNode(c)->getId()));
			}
			pOwner->setSteering(Steering::ARRIVETOALLSTEERING, nodePositions);
			pOwner->isFinished = false;
		}
	}
	else if(gpGame->getAIFight()){
		if(!pOwner->poweredUp()){
			std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( RUNAWAY_TRANSITION );
			if( iter != mTransitions.end() )//found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}
		std::vector<Unit*> units = gpGame->getUnitManager()->getAllUnits();
		for(int i = 0; i < units.size(); i++){
			if(units[i]->getType() == Unit::TYPE::ENEMY){
				Unit* enemy = units[i];
				Vector2D diff = enemy->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

				auto distance = diff.getLength();
				if(distance < mFollowRange){
					if(pOwner->isFinished){
						pOwner->isFinished = false;
						PositionData data = pOwner->getPositionComponent()->getData();
						PositionData targetData = enemy->getPositionComponent()->getData();
						GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
						GridPathfinder* pPathfinder = pGame->getPathfinder();
						GridGraph* pGridGraph = pGame->getGridGraph();
						Grid* pGrid = pGame->getGrid();

						int fromIndex = pGrid->getSquareIndexFromPixelXY(data.pos.getX(), data.pos.getY());
						int toIndex = pGrid->getSquareIndexFromPixelXY(targetData.pos.getX(), targetData.pos.getY());

						Node* pFromNode = pGridGraph->getNode(fromIndex);
						Node* pToNode = pGridGraph->getNode(toIndex);
						Path* pPath = pGame->getPathfinder()->findPath(pToNode, pFromNode);
						std::vector<Vector2D> nodePositions;
						if(!pPath){
							return NULL;
						}
						for (int c = 0; c < pPath->getNumNodes(); c++){
							nodePositions.push_back(pGrid->getULCornerOfSquare(pPath->peekNode(c)->getId()));
						}
						pOwner->setSteering(Steering::ARRIVETOALLSTEERING, nodePositions);
						pOwner->isFinished = false;
					}
					return NULL;
				}
			}
		}
		std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( WANDER_TRANSITION );
		if( iter != mTransitions.end() )//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}

	//else just return null
	return NULL;
}