#pragma once

#include "Pathfinder.h"
class GridGraph;
class GridVisualizer;
class GraphicsBuffer;
class PathfindingDebugContent;

//==Base pathfinder class to be used with the gridgraph grid to find paths==//
class GridPathfinder:public Pathfinder
{
public:
	friend class PathfindingDebugContent;

	GridPathfinder( GridGraph* pGraph );
	virtual ~GridPathfinder();

	virtual Path* findPath( Node* pFrom, Node* pTo ) = 0;
	
#ifdef VISUALIZE_PATH
	//just for visualization
public:
	void drawVisualization( Grid* pGrid, GraphicsBuffer* pDest );
protected:
	std::vector<Node*> mVisitedNodes;
	Path* mpPath;
	GridVisualizer* mpVisualizer;
	bool isEndNodeValid(Node* endNode);
#endif

	double mTimeElapsed;
};