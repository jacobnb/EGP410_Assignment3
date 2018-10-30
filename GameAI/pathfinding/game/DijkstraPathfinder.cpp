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
	PriorityQueue<Node*, std::vector<Node*>, Compare> nodesToVisit;
	Node* startNode = pFrom;
	startNode->setCost(0);
	nodesToVisit.push(startNode);

	//Will probably have to change this.
#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear(); //This tracks # of nodes processed
	mVisitedNodes.push_back(pFrom);
#endif

	Path* pPath = new Path(); 
	//this is for visualization but also acts as the closed list. 

	Node* pCurrentNode = nullptr;
	//end node added.
	bool toNodeAdded = false;

	//start iterating through nodes
	while (pCurrentNode != pTo && nodesToVisit.size() > 0) {
		pCurrentNode = nodesToVisit.top(); //access the top element
		nodesToVisit.pop(); //remove node, doesn't return it

		pPath->addNode(pCurrentNode);

		//get connections from current Node
		std::vector<Connection*> connections = mpGraph->getConnections(pCurrentNode->getId());

		for (unsigned int i = 0; i < connections.size(); i++) { 
			//for each neighbor of current node
			Connection* pConnection = connections[i];

			//set up node.
			Node* pTempToNode = connections[i]->getToNode();
			auto cost = pConnection->getCost() + pCurrentNode->getCost();

			//if node is in open list update it
			if (nodesToVisit.find(pTempToNode) != nodesToVisit.end()) {
				if (pTempToNode->getCost() > cost) { //if shorter path has been found.
					pTempToNode->setCost(cost); //set cost
					pTempToNode->setPrevNode(pCurrentNode); //set previous node
				}
			}
			//if note is not in closed list, set values
			else if (!pPath->containsNode(pTempToNode)) {
				pTempToNode->setCost(cost);
				pTempToNode->setPrevNode(pCurrentNode);
			}

			if (!toNodeAdded && //if end not found
				!pPath->containsNode(pTempToNode) &&  //node is not in path
				nodesToVisit.find(pTempToNode) == nodesToVisit.end()
				//node is not in nodesTovisit
				) {
				nodesToVisit.push(pTempToNode);

				if (pTempToNode->getId() == pTo->getId()) { //found node, not 100% shortest path.
					toNodeAdded = true;
				}
#ifdef VISUALIZE_PATH
				mVisitedNodes.push_back(pTempToNode);
#endif
			}
		}
	}

	Node* lastPathNode = pTo;
#ifdef VISUALIZE_PATH
	delete pPath;
	pPath = new Path();
	while (lastPathNode != pFrom) {
		pPath->addNode(lastPathNode);
		lastPathNode = lastPathNode->getPrevNode();
		if (lastPathNode == nullptr) {
			lastPathNode = pFrom;
		}
	}
#endif

	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
		mpPath = pPath;
#endif
	return pPath;
}
