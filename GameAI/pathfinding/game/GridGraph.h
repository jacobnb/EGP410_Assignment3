#pragma once

#include "Graph.h"

class Grid;

//==Graph that is in a grid shape, used for all current pathfinding.==//
class GridGraph:public Graph
{
public:
	GridGraph( Grid* pGrid );
	virtual ~GridGraph();

	void setGrid( Grid* pGrid ) { mpGrid = pGrid; };
	Grid* getGrid() { return mpGrid; };
	void init();

private:
	Grid* mpGrid;
	float mConnectionCost = 1.0f;
};