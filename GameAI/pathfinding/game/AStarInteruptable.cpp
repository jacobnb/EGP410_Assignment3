#include "AStarInteruptable.h"
#include <PerformanceTracker.h>
#include "Connection.h"
#include "Path.h"
#include "Game.h"
#include "GameApp.h"
#include "GridGraph.h"
#include "Grid.h"
#include "Vector2D.h"
#include "Timer.h"

AStarInteruptable::AStarInteruptable(Graph * pGraph) :
	GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif
}

AStarInteruptable::~AStarInteruptable()
{
	for (int i = 0; i < mClosedList.size(); i++) {
		delete mClosedList[i];
	}
	mClosedList.clear();
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

Path * AStarInteruptable::findPath(Node * pFrom, Node * pTo, float timeToRun)
{
	Timer* pTimer = new Timer();
	pTimer->start();
	//New path setup.
	if (mpFromNode != pFrom || mpToNode != pTo) {
		//== Handle if the user clicks on the black + ==//
		if (!isEndNodeValid(pTo)) {
			Path* nullPath = new Path();
			nullPath->addNode(pFrom);
			return nullPath;
		}
		//== If user clicks on the same node ==//
		if (pFrom == pTo) {
			Path* nullPath = new Path();
			nullPath->addNode(pFrom);
			return nullPath;
		}

		//Set up values
		mpToNodeStruct = nullptr; //is end node added.
		mpFromNode = pFrom;
		mpToNode = pTo;
		
		//clear closed list.
		for (int i = 0; i < mClosedList.size(); i++) {
			delete mClosedList[i];
		}
		mClosedList.clear();

		//Add starting node to open list
		mpCurrentNodeStruct = new NodeStruct(pFrom);
		mNodesToVisit.push(mpCurrentNodeStruct);
		
#ifdef VISUALIZE_PATH
		mVisitedNodes.clear(); //This list is only useful for numNodes Processed
		mVisitedNodes.push_back(pFrom);
#endif
	}

	//These are used inside the while loop.
	bool notInClosedList = true;
	bool tempToNodeIsInOpenList = false;
	//Start iterating.
	while (mpCurrentNodeStruct->mpThisNode != pTo && mNodesToVisit.size() > 0) {
		mpCurrentNodeStruct = mNodesToVisit.top(); //access the top element
		mNodesToVisit.pop(); //remove node, doesn't return it

		mClosedList.push_back(mpCurrentNodeStruct);
		//get connections from current Node
		std::vector<Connection*> connections = mpGraph->getConnections(mpCurrentNodeStruct->mpThisNode->getId());
		NodeStruct* pTempToNodeStruct;

		for (unsigned int i = 0; i < connections.size(); i++) {
			//for each neighbor of current node
			Connection* pConnection = connections[i];
			//set up node.
			//Checks for node in open list, returns iter.
			auto structIter = mNodesToVisit.findStruct(pConnection->getToNode());
			if (structIter == mNodesToVisit.end()) {
				//if not in open list;
				tempToNodeIsInOpenList = false;
				pTempToNodeStruct = new NodeStruct(pConnection->getToNode());
			}
			else {
				tempToNodeIsInOpenList = true;
				pTempToNodeStruct = *structIter;
			}
			auto cost = pConnection->getCost() + mpCurrentNodeStruct->mCost;
			//if heuristic becomes expensive, should optimize by not re-caulculating.
			auto hCost = nodeHeuristic(pTempToNodeStruct->mpThisNode, pTo);
			auto totalCost = cost + hCost;

			//Check for shorter path or init values.
			//If it's in the open list
			if (tempToNodeIsInOpenList) {
				if (pTempToNodeStruct->totalCost() > totalCost) { //if shorter path has been found.
					pTempToNodeStruct->mCost = cost;
					pTempToNodeStruct->mHeuristicCost = hCost;
					pTempToNodeStruct->mpPrevNodeStruct = mpCurrentNodeStruct;
					mNodesToVisit.rebalance();
				}
			}
			else {
				//check for node in closed list
				std::vector<NodeStruct*>::iterator iter = mClosedList.begin();
				notInClosedList = true;
				while (iter != mClosedList.end() && notInClosedList) {
					if ((*iter)->mpThisNode == pTempToNodeStruct->mpThisNode) {
						notInClosedList = false;
					}
					iter++;
				}
				if (!notInClosedList) {
					// this should leave the iter pointing at the correct nodestruct if found.
					iter--;
				}

				//set values
				if (notInClosedList) {
					//once node is not in to visit list or in path.
					pTempToNodeStruct->mCost = cost;
					pTempToNodeStruct->mHeuristicCost = hCost;
					pTempToNodeStruct->mpPrevNodeStruct = mpCurrentNodeStruct;
				}
				//update values and remove from closed list.
				else {
					if (pTempToNodeStruct->mCost > cost) { //if shorter path has been found.
						//Not sure if this will remove from list or delete pointer
						mClosedList.erase(iter);
						notInClosedList = true;
						pTempToNodeStruct->mCost = cost;
						pTempToNodeStruct->mHeuristicCost = hCost;
						pTempToNodeStruct->mpPrevNodeStruct = mpCurrentNodeStruct;

					}
				}
			}


			//Add node to the open list
			if (mpToNodeStruct == nullptr && //if end not found
				notInClosedList &&  //node is not in path
				!tempToNodeIsInOpenList //node is not in nodesTovisit
				) {
				mNodesToVisit.push(pTempToNodeStruct);

				if (pTempToNodeStruct->mpThisNode->getId() == pTo->getId()) { //found node, not 100% shortest path.
					mpToNodeStruct = pTempToNodeStruct;
				}
#ifdef VISUALIZE_PATH
				mVisitedNodes.push_back(pTempToNodeStruct->mpThisNode);
#endif
			}
		}
		//If out of time, return nullptr;
		if (pTimer->getElapsedTime() > timeToRun) {
			delete pTimer;
			return nullptr;
		}
	}

#ifdef VISUALIZE_PATH
	if (mpToNodeStruct == nullptr) {
		std::cout << "You Broke it";
		Path* nullPath = new Path();
		nullPath->addNode(pFrom);
		return nullPath;
	}
	delete mpPath;
	Path* pPath = new Path();
	while (mpToNodeStruct->mpThisNode != pFrom) {
		pPath->addNode(mpToNodeStruct->mpThisNode);
		mpToNodeStruct = mpToNodeStruct->mpPrevNodeStruct;
		if (mpToNodeStruct == NULL) {
			throw "Broken Path in AStar";
		}
	}
#endif
	//should probably delete the closed list here?
	delete pTimer;
	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");


#ifdef VISUALIZE_PATH
	mpPath = pPath;

#endif
	return pPath;
}

Path * AStarInteruptable::findPath(Node * pFrom, Node * pTo)
{
	Path* pPath = nullptr;
	while (pPath == nullptr) {
		pPath = findPath(pFrom, pTo, 5.00f);
	}
	return pPath;
}

float AStarInteruptable::pxHeuristic(Node * pFrom, Node * pTo)
{
	//Heuristic inadmisable it's in pixels while node cost is in connection cost.
	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
	Vector2D diff = pGrid->getULCornerOfSquare(pFrom->getId()) -
		pGrid->getULCornerOfSquare(pTo->getId());

	return diff.getLength(); //divide by square size in px to get heuristic in connection costs
}

float AStarInteruptable::nodeHeuristic(Node * pFrom, Node * pTo)
{
	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
	//this assumes square nodes
	auto pxDist = pxHeuristic(pFrom, pTo);
	auto nodeDist = pxDist * pGrid->getGridWidth() / pGrid->getPixelWidth();
	return nodeDist;
}

