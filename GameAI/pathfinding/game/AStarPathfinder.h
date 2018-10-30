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
	float heuristic(Node* pFrom, Node* pTo);
};