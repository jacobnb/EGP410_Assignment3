#pragma once

#include "GridPathfinder.h"
#include <vector>


class Path;
class Graph;
class GraphicsBuffer;
class Grid;

//==Find a path using DFS (or BFS) between two nodes==//
class DepthFirstPathfinder:public GridPathfinder
{
public:
	DepthFirstPathfinder( Graph* pGraph );
	~DepthFirstPathfinder();

	Path* findPath( Node* pFrom, Node* pTo );//make sure to delete the path when you are done!

private:
};

