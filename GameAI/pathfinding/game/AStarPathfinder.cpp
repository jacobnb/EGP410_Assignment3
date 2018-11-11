#include "AStarPathfinder.h"
#include <PerformanceTracker.h>
#include "PriorityQueue.h"
#include "Connection.h"
#include "Compare.h"
#include "Path.h"
#include "Game.h"
#include "GameApp.h"
#include "GridGraph.h"
#include "Grid.h"
#include "Vector2D.h"

AStarPathfinder::AStarPathfinder(Graph* pGraph) :
	GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif
}

AStarPathfinder::~AStarPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

Path * AStarPathfinder::findPath(Node * pFrom, Node * pTo)
{

	//== This chunk of code handles if the user clicks on the black + ==//
	if (!isEndNodeValid(pTo)) {
		return nullptr;
	}

	if(static_cast<GameApp*>(gpGame)->FindPath(pFrom, pTo)){
		return static_cast<GameApp*>(gpGame)->FindPath(pFrom, pTo);
	}

	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");
	//set up Open queue and add starting node.
	PriorityQueue<NodeStruct*, std::vector<NodeStruct*>, CompareAStarStruct> nodesToVisit;
	NodeStruct* pCurrentNodeStruct = new NodeStruct(pFrom);
	nodesToVisit.push(pCurrentNodeStruct);

#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear(); //This list is only useful for numNodes Processed
	mVisitedNodes.push_back(pFrom);
#endif

	std::vector<NodeStruct*> closedList;
	bool notInClosedList = true;
	bool tempToNodeIsInOpenList = false;
	//end node added.
	NodeStruct* toNodeStruct = nullptr;

	//Start iterating.
	while (pCurrentNodeStruct->mpThisNode != pTo && nodesToVisit.size() > 0) {
		pCurrentNodeStruct = nodesToVisit.top(); //access the top element
		nodesToVisit.pop(); //remove node, doesn't return it

		//Are we balanceing on cost? does it work?

		closedList.push_back(pCurrentNodeStruct);
		//get connections from current Node
		std::vector<Connection*> connections = mpGraph->getConnections(pCurrentNodeStruct->mpThisNode->getId());
		NodeStruct* pTempToNodeStruct;

		for (unsigned int i = 0; i < connections.size(); i++) { 
			//for each neighbor of current node
			Connection* pConnection = connections[i];

			//set up node.
			//Checks for node in open list, returns iter.
			auto structIter = nodesToVisit.findStruct(pConnection->getToNode());
			if (structIter == nodesToVisit.end()) {
				//if not in open list;
				tempToNodeIsInOpenList = false;
				pTempToNodeStruct = new NodeStruct(pConnection->getToNode());
			}
			else {
				tempToNodeIsInOpenList = true;
				pTempToNodeStruct = *structIter;
			}
			auto cost = pConnection->getCost() + pCurrentNodeStruct->mCost;
			//if heuristic becomes expensive, should optimize by not re-caulculating.
			auto hCost = nodeHeuristic(pTempToNodeStruct->mpThisNode, pTo);
			auto totalCost = cost + hCost;

			//Check for shorter path or init values.
			//If it's in the open list
			if (tempToNodeIsInOpenList) {
				if (pTempToNodeStruct->totalCost() > totalCost) { //if shorter path has been found.
					pTempToNodeStruct->mCost = cost;
					pTempToNodeStruct->mHeuristicCost = hCost;
					pTempToNodeStruct->mpPrevNodeStruct = pCurrentNodeStruct;
					nodesToVisit.rebalance();
				}
			}
			else {
				//check for node in closed list
				std::vector<NodeStruct*>::iterator iter = closedList.begin();
				notInClosedList = true;
				while (iter != closedList.end() && notInClosedList) {
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
					pTempToNodeStruct->mpPrevNodeStruct = pCurrentNodeStruct;
				}
				//update values and remove from closed list.
				else {
					if (pTempToNodeStruct->mCost > cost) { //if shorter path has been found.
						//Not sure if this will remove from list or delete pointer
						closedList.erase(iter); 
						pTempToNodeStruct->mCost = cost;
						pTempToNodeStruct->mHeuristicCost = hCost;
						pTempToNodeStruct->mpPrevNodeStruct = pCurrentNodeStruct;

					}
				}
			}


			//Add node to the open list
			if (toNodeStruct == nullptr && //if end not found
				notInClosedList &&  //node is not in path
				!tempToNodeIsInOpenList //node is not in nodesTovisit
				) {
				nodesToVisit.push(pTempToNodeStruct);

				if (pTempToNodeStruct->mpThisNode->getId() == pTo->getId()) { //found node, not 100% shortest path.
					toNodeStruct = pTempToNodeStruct;
				}
#ifdef VISUALIZE_PATH
				mVisitedNodes.push_back(pTempToNodeStruct->mpThisNode);
#endif
			}
		}
	}

#ifdef VISUALIZE_PATH
	Path* pPath = new Path();
	while (toNodeStruct->mpThisNode != pFrom) {
		pPath->addNode(toNodeStruct->mpThisNode);
		toNodeStruct = toNodeStruct->mpPrevNodeStruct;
		if (toNodeStruct == nullptr) {
			throw "Broken Path in AStar";
		}
	}
#endif

	static_cast<GameApp*>(gpGame)->CachePath(pFrom, pTo, pPath);

	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
	mpPath = pPath;

#endif
	return pPath;
}

float AStarPathfinder::pxHeuristic(Node * pFrom, Node * pTo)
{
	//Heuristic inadmisable it's in pixels while node cost is in connection cost.
	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
	Vector2D diff = pGrid->getULCornerOfSquare(pFrom->getId()) -
		pGrid->getULCornerOfSquare(pTo->getId());

	return diff.getLength(); //divide by square size in px to get heuristic in connection costs
}

float AStarPathfinder::nodeHeuristic(Node * pFrom, Node * pTo)
{
	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
	//this assumes square nodes
	float pxToNodeFactor = pGrid->getGridWidth() / pGrid->getPixelWidth();
	auto pxDist = pxHeuristic(pFrom, pTo);
	auto nodeDist = pxDist * pxToNodeFactor;
	return nodeDist;
}

