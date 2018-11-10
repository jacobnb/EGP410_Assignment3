#pragma once
#include "GridPathfinder.h"


class Path;
class Graph;
class GraphicsBuffer;
class Grid;

//create flow field
class FlowFieldPathfinder: public GridPathfinder {
public:
	FlowFieldPathfinder(Graph* pGraph);
	~FlowFieldPathfinder();
	Path* findPath(Node* pFrom, Node* pTo); //don't forget to delete path
private:

};