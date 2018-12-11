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
	if(!mPlayer){
		Unit* player = gpGame->getUnitManager()->getPlayerUnit();
		Unit* owner = gpGame->getUnitManager()->getUnit(mOwnerID);
		//owner->setSteering(Steering::PATHFINDFOLLOW, ZERO_VECTOR2D, player->GetID());
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
		else if(pOwner->isFinished){
			PositionData data = pOwner->getPositionComponent()->getData();
			PositionData targetData = gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getData();
			GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
			GridPathfinder* pPathfinder = pGame->getPathfinder();
			GridGraph* pGridGraph = pGame->getGridGraph();
			Grid* pGrid = pGame->getGrid();

			int fromIndex = pGrid->getSquareIndexFromPixelXY(data.pos.getX(), data.pos.getY());
			int toIndex = pGrid->getSquareIndexFromPixelXY(targetData.pos.getX(), data.pos.getY());

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
		}
	}
	//else just return null
	return NULL;
}