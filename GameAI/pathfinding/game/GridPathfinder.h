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
	friend class GameApp;

	GridPathfinder( GridGraph* pGraph );
	virtual ~GridPathfinder();

	virtual Path* findPath( Node* pFrom, Node* pTo ) = 0;
	struct NodeStruct {
		NodeStruct(Node* node) {
			mpThisNode = node;
			mCost = 0;
			mHeuristicCost = 0;
			mpPrevNodeStruct = nullptr;
		}
		Node* mpThisNode;
		float mCost;
		float mHeuristicCost;
		NodeStruct* mpPrevNodeStruct;
		float totalCost() {
			return mCost + mHeuristicCost;
		}
		~NodeStruct() {}; //struct owns neither of the pointers.
	};
#ifdef VISUALIZE_PATH
	//just for visualization
public:
	void drawVisualization( Grid* pGrid, GraphicsBuffer* pDest );
	Path* mpPath;
protected:
	std::vector<Node*> mVisitedNodes;
	GridVisualizer* mpVisualizer;
	bool isEndNodeValid(Node* endNode);
#endif
protected:
	double mTimeElapsed;
};