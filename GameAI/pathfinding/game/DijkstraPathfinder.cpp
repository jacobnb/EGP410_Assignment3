#include "DijkstraPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include "PriorityQueue.h"
#include "Compare.h"

DijkstraPathfinder::DijkstraPathfinder(Graph * pGraph) :
	GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif

}

DijkstraPathfinder::~DijkstraPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

Path * DijkstraPathfinder::findPath(Node * pFrom, Node * pTo)
{
	//== This chunk of code handles if the user clicks on the black + ==//
	if (!isEndNodeValid(pTo)) {
		return nullptr;
	}

	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");
	//set up Open queue and add starting node.
	PriorityQueue<NodeStruct*, std::vector<NodeStruct*>, CompareDijkstraStruct> nodesToVisit;
	NodeStruct* pCurrentNodeStruct = new NodeStruct(pFrom);
	nodesToVisit.push(pCurrentNodeStruct);

#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear(); //This tracks # of nodes processed
	mVisitedNodes.push_back(pFrom);
#endif

	std::vector<NodeStruct*> closedList;
	bool notInClosedList = true; //used in search later.

	//set when end node added.
	NodeStruct* toNodeStruct = nullptr;

	//start iterating through nodes
	while (pCurrentNodeStruct->mpThisNode != pTo && nodesToVisit.size() > 0) {
		pCurrentNodeStruct = nodesToVisit.top(); //access the top element
		nodesToVisit.pop(); //remove node, doesn't return it

		closedList.push_back(pCurrentNodeStruct);

		//get connections from current Node
		std::vector<Connection*> connections = mpGraph->getConnections(pCurrentNodeStruct->mpThisNode->getId());

		for (unsigned int i = 0; i < connections.size(); i++) { 
			//for each neighbor of current node
			Connection* pConnection = connections[i];

			//set up node.
			NodeStruct* pTempToNodeStruct = new NodeStruct(connections[i]->getToNode());
			auto cost = pConnection->getCost() + pCurrentNodeStruct->mCost;
			notInClosedList = true; //set later if not is not in open list.

			//if node is in open list update it
			//Need to update pTempToNodeStruct so that it points at the same node.
			if (nodesToVisit.find(pTempToNodeStruct) != nodesToVisit.end()) {
				if (pTempToNodeStruct->mCost > cost) { //if shorter path has been found.
					pTempToNodeStruct->mCost = cost; //set cost
					pTempToNodeStruct->mpPrevNodeStruct = pCurrentNodeStruct; //set previous node
				}
			}
			//if node is not in closed or open list, set values
			else {
				//Check for node in closed list.
				std::vector<NodeStruct*>::iterator iter = closedList.begin();
				while (iter != closedList.end()) {
					if ((*iter) == pTempToNodeStruct) {
						iter == closedList.end() - 1;
						notInClosedList = false;
					}
					iter++;
				}
				//Set values
				if (notInClosedList) {
					pTempToNodeStruct->mCost = cost;
					pTempToNodeStruct->mpPrevNodeStruct = pCurrentNodeStruct;
				}
			}

			//If end not found, not in closed list, and not in open list.
			//Add to open list and check if it's the end.
			if (toNodeStruct == nullptr &&
				notInClosedList && 
				nodesToVisit.find(pTempToNodeStruct) == nodesToVisit.end()
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

	NodeStruct* lastPathNodeStruct;
#ifdef VISUALIZE_PATH
	Path* pPath = new Path();
	while (lastPathNodeStruct->mpThisNode != pFrom) {
		pPath->addNode(lastPathNodeStruct->mpThisNode);
		lastPathNodeStruct = lastPathNodeStruct->mpPrevNodeStruct;
		if (lastPathNodeStruct == nullptr) {
			throw "Broken Path in Dijkstra";
		}
	}
#endif

	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
		mpPath = pPath;
#endif
	for (int i = 0; i < closedList.size(); i++) {
		delete closedList[i];
	}
	nodesToVisit.clear();
	return pPath;
}
