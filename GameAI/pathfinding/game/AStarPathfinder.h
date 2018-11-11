#pragma once
#include "GridPathfinder.h"


class Path;
class Graph;
class GraphicsBuffer;
class Grid;

//Find a path using the A* algorithm.
class AStarPathfinder : public GridPathfinder {
public:
	AStarPathfinder(Graph* pGraph);
	~AStarPathfinder();
	Path* findPath(Node* pFrom, Node* pTo); //don't forget to delete path
private:
	//linear distance in pixels.
	float pxHeuristic(Node* pFrom, Node* pTo);
	//linear distance in nodes
	float nodeHeuristic(Node* pFrom, Node* pTo);
};