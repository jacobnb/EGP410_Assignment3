#include "PathfindManager.h"
#include "GameApp.h"
#include "Game.h"
#include "Pathfinder.h"
#include "GridPathfinder.h"
#include "Grid.h"
#include "GridGraph.h"
PathfindManager::PathfindManager()
{
	currentRequest = nullptr;
}

PathfindManager::~PathfindManager()
{
	//not sure this is necessary but shouldn't hurt.
	while (!mPathFindRequests.empty()) {
		delete mPathFindRequests.front();
		mPathFindRequests.pop();
	}
}

void PathfindManager::update(float timeToRun)
{
	if (!currentRequest) {
		if (!mPathFindRequests.empty()) {
			currentRequest = mPathFindRequests.front();
			mPathFindRequests.pop();
		}
		else {
			return;
		}
	}
	//Current request exists.
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Path* pPath = pGame->getPathfinder()->findPath(currentRequest->pTo, currentRequest->pFrom, timeToRun);
	if (pPath != nullptr) {
		//if a path is found, update steering
		pGame->UpdateSteering(currentRequest->mId, pGame->getPathfinder()->mpPath);
		currentRequest = nullptr;
	}
	else {
		currentRequest = nullptr;
	}
}

void PathfindManager::addPathToFind(Node * pFromNode, Node * pToNode, int mIdNum)
{
	mPathFindRequests.push(new PathRequest(pFromNode, pToNode, mIdNum));
}
