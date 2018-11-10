#include "FlowFieldPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include "PriorityQueue.h"
#include "Compare.h"
#include <list>
FlowFieldPathfinder::FlowFieldPathfinder(Graph * pGraph) :
	GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif

}

FlowFieldPathfinder::~FlowFieldPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

//pfrom is not needed

//fix diagonalcost
// make sure all nodes that are walls have really really high costs
Path * FlowFieldPathfinder::findPath(Node * pFrom, Node * pTo)
{
	//== This chunk of code handles if the user clicks on the black + ==//
	if (!isEndNodeValid(pTo)) {
		return nullptr;
	}

	std::list<Node*> nodesToVisit;

	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");
	
	Node* startNode = pTo;
	startNode->setCost(0);
	nodesToVisit.push_back(startNode);

	//Will probably have to change this.
#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear(); //This tracks # of nodes processed
#endif

	Path* pPath = new Path(); 
	//this is for visualization but also acts as the closed list. 

	Node* pCurrentNode = nullptr;
	//end node added.
	bool toNodeAdded = false;

	//start iterating through nodes
	while (nodesToVisit.size() > 0) {
		pCurrentNode = nodesToVisit.front();//access the top element
		nodesToVisit.pop_front(); //remove node, doesn't return it
		pPath->addNode(pCurrentNode);
		//get connections from current Node
		std::vector<Connection*> connections = mpGraph->getConnections(pCurrentNode->getId());

		for (unsigned int i = 0; i < connections.size(); i++) { 
			//for each neighbor of current node
			Connection* pConnection = connections[i];

			//set up node.
			Node* pTempToNode = connections[i]->getToNode();
			auto cost = pConnection->getCost() + pCurrentNode->getCost();

			if (!pPath->containsNode(pTempToNode) &&
				find(nodesToVisit.begin(), nodesToVisit.end(), pTempToNode) == nodesToVisit.end())
			{
				pTempToNode->setCost(cost);
				nodesToVisit.push_back( pTempToNode );

#ifdef VISUALIZE_PATH
				mVisitedNodes.push_back(pTempToNode);
#endif
			}
		}
	}

	for each (Node* node in mVisitedNodes)
	{
		std::vector<Connection*> connections = mpGraph->getConnections(node->getId());
		Node* smallest = connections[0]->getToNode();
		for (unsigned int i = 1; i < connections.size(); i++) {
			//for each neighbor of current node
			Node* pTempToNode = connections[i]->getToNode();
			if (smallest->getCost() > pTempToNode->getCost())
			{
				smallest = pTempToNode;
			}
		}

		node->setDir(smallest);
	}
	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
		mpPath = pPath;
#endif
	return pPath;
}
