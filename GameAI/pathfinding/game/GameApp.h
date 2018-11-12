#pragma once

/*Game - class to hold all game related info.

Dean Lawson
Champlain College
2011
*/

#include "Game.h"
#include <vector>
#include <map>

//forward declarations
class GraphicsBuffer;
class Sprite;
class KinematicUnit;
class GameMessageManager;
class Grid;
class GridVisualizer;
class GridGraph;
class GridPathfinder;
class DebugDisplay;
class InputSystem;
class Path;
class Node;
class PathfindManager;

const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame
const int UNIT_SIZE = 10;

class GameApp: public Game
{
public:
	GameApp();
	~GameApp();

	virtual bool init();
	virtual void cleanup();

	//game loop
	virtual void beginLoop();
	virtual void processLoop();
	virtual bool endLoop();

	//accessors
	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline GridVisualizer* getGridVisualizer() { return mpGridVisualizer; };
	inline GridPathfinder* getPathfinder() { return mpPathfinder; };
	inline Grid* getGrid() { return mpGrid; };
	inline GridGraph* getGridGraph() { return mpGridGraph; };
	inline InputSystem* getInputSystem() { return mpInputSystem; };

	//search algorithm
	void changeToDijkstra();
	void changeToDFS();
	void changeToAStar();
	void changeToInteruptable();
	void adjustFlockUI();
	void UpdateSteering(int index, Path* path);
	void ClearPathMap();
	void MakeUnits();
	void CachePath(Node* n1, Node* n2, Path* path);
	Path* FindPath(Node* n1, Node* n2);

	void generatePath(Node* pFromNode, Node* pToNode, int mIdNum);
private:
	//Interruptable pathfinder stuff//
	bool shouldFindPath = false;
	Node* pTo, *pFrom;
	int mId;
	PathfindManager* mpPathfindManager;
	//Create an array with length > max MID
	//use MID as index, and pass in nodes and stuff. 

	//end of Interruptable pathfinder stuff//

	std::string truncateFloat(float num);

	GameMessageManager* mpMessageManager;
	Grid* mpGrid;
	GridGraph* mpGridGraph;
	InputSystem* mpInputSystem;
	GridVisualizer* mpGridVisualizer;

	std::map<std::string, Path*> pathMap;

	DebugDisplay* mpDebugDisplay;
	GridPathfinder* mpPathfinder;

};

