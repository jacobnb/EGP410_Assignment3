#include "CandyState.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "GameApp.h"
#include "Grid.h"
#include "GridGraph.h"
#include "GridPathfinder.h"
#include "Path.h"

void CandyState::onEntrance(){
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	pOwner->isFinished = true;
}

void CandyState::onExit(){

}

StateTransition* CandyState::update(){
	//if you are x distance away from a candy then move towards it
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	std::vector<Unit*> units = gpGame->getUnitManager()->getAllUnits();
	if(mPlayer){
		return grabMightyCandy();
	}
	else {
		return grabEnemyFood();
	}

	//else just return null
	return NULL;
}

StateTransition* CandyState::grabEnemyFood(){
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	std::vector<Unit*> units = gpGame->getUnitManager()->getAllUnits();
	int closestDistance = 5000;
	Unit* closestEnemy = NULL;
	for(int i = 0; i < units.size(); i++){
		if(units[i]->getType() == Unit::TYPE::ENEMY_FOOD){
			Unit* food = units[i];
			Vector2D diff = food->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

			auto distance = diff.getLength();
			if(distance < mFollowRange){
				if(closestDistance > distance){
					closestDistance = distance;
					closestEnemy = food;
				}
			}
		}
	}
	if(closestEnemy){
		if(pOwner->isFinished){
			pOwner->isFinished = false;
			//Pathfind to the candy
			grabEnemyFood();
			PositionData data = pOwner->getPositionComponent()->getData();
			PositionData targetData = closestEnemy->getPositionComponent()->getData();
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

	//Else just switch back to wander
	std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( WANDER_TRANSITION );
	if( iter != mTransitions.end() )//found?
	{
		StateTransition* pTransition = iter->second;
		return pTransition;
	}
}

StateTransition* CandyState::grabMightyCandy(){
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	std::vector<Unit*> units = gpGame->getUnitManager()->getAllUnits();
	int closestDistance = 5000;
	Unit* closestEnemy = NULL;
	for(int i = 0; i < units.size(); i++){
		if(units[i]->getType() == Unit::TYPE::MIGHTY_CANDY){
			Unit* food = units[i];
			Vector2D diff = food->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();
			auto distance = diff.getLength();
			if(distance < mFollowRange){
				if(closestDistance > distance){
					closestDistance = distance;
					closestEnemy = food;
				}
			}
		}
	}
	if(closestEnemy){
		if(pOwner->isFinished){
			pOwner->isFinished = false;
			//Pathfind to the candy
			grabEnemyFood();
			PositionData data = pOwner->getPositionComponent()->getData();
			PositionData targetData = closestEnemy->getPositionComponent()->getData();
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

	//Else just switch back to wander
	std::map<TransitionType, StateTransition*>::iterator iter = mTransitions.find( WANDER_TRANSITION );
	if( iter != mTransitions.end() )//found?
	{
		StateTransition* pTransition = iter->second;
		return pTransition;
	}
}