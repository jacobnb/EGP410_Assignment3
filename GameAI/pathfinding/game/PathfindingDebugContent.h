#pragma once

#include "DebugContent.h"

class GridPathfinder;

//==Stores Debug content about the pathfinder==//
class PathfindingDebugContent:public DebugContent
{
public:
	PathfindingDebugContent( GridPathfinder* pPathfinder );
	~PathfindingDebugContent(){};

	std::string getDebugString();

private:
	GridPathfinder* mpPathfinder;
};