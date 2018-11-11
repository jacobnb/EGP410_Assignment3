#pragma once
#include "GridPathfinder.h"
#include "PriorityQueue.h"
#include "Compare.h"

class Path;
class Graph;
class GraphicsBuffer;
class Grid;

//Find a path using the A* algorithm.
class AStarInteruptable : public GridPathfinder {
public:
	AStarInteruptable(Graph* pGraph);
	~AStarInteruptable();
	Path* findPath(Node* pFrom, Node* pTo, float timeToRun); //don't forget to delete path
private:
	//linear distance in pixels.
	float pxHeuristic(Node* pFrom, Node* pTo);
	//linear distance in nodes
	float nodeHeuristic(Node* pFrom, Node* pTo);
	PriorityQueue<NodeStruct*, std::vector<NodeStruct*>, CompareAStarStruct> mNodesToVisit;
	std::vector<NodeStruct*> mClosedList;
	Node* mpFromNode;
	Node* mpToNode;
	NodeStruct* mpCurrentNodeStruct;
	NodeStruct* mpToNodeStruct;
};