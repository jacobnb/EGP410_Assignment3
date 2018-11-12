#include "pathfindingMessage.h"
#include "Game.h"
#include "GameApp.h"
#include "GridPathfinder.h"
#include "Grid.h"
#include "GridGraph.h"

PathfindingMessage::PathfindingMessage(const Vector2D& center, const int id)
	:GameMessage(PATH_TO_MESSAGE)
	, mCenter(center)
	, mId(id)
{
}

PathfindingMessage::~PathfindingMessage()
{
}

void PathfindingMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (pGame != NULL)
	{
		GridPathfinder* pPathfinder = pGame->getPathfinder();
		GridGraph* pGridGraph = pGame->getGridGraph();
		Grid* pGrid = pGame->getGrid();

		int toIndex = pGrid->getSquareIndexFromPixelXY((int)mCenter.getX(), (int)mCenter.getY());

		Node* pCenterNode = pGridGraph->getNode(toIndex);

		//with flow field, it doesnt need a from node
		pPathfinder->findPath(pCenterNode, pCenterNode);
		pGame->UpdateSteering(mId, pPathfinder->mpPath);
	}
}
