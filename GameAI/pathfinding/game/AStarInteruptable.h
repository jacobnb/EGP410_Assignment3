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
	Path* findPath(Node* pFrom, Node* pTo); //for testing.
private:
	//linear distance in pixels.
	float pxHeuristic(Node* pFrom, Node* pTo);
	//linear distance in nodes
	float nodeHeuristic(Node* pFrom, Node* pTo);
	
	PriorityQueue<NodeStruct*, std::vector<NodeStruct*>, CompareAStarStruct> mNodesToVisit;
	std::vector<NodeStruct*> mClosedList;
	NodeStruct* mpCurrentNodeStruct;
	NodeStruct* mpToNodeStruct;
	//class does not own these. DO NOT DELETE
	Node* mpFromNode;
	Node* mpToNode;
};