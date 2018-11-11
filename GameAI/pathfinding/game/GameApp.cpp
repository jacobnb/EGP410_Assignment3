#include "Game.h"
#include "GameApp.h"
#include "GameMessageManager.h"
#include "PathToMessage.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include <Vector2D.h>
#include "Grid.h"
#include "GridGraph.h"
#include "Connection.h"
#include "Path.h"
#include "DepthFirstPathfinder.h"
#include "DijkstraPathfinder.h"
#include "AStarPathfinder.h"
#include "Pathfinder.h"
#include "GridPathfinder.h"
#include "GridVisualizer.h"
#include "DebugDisplay.h"
#include "PathfindingDebugContent.h"
#include "InputSystem.h"
#include "Unit.h"
#include "UnitManager.h"
#include "ComponentManager.h"
#include "DataLoader.h"
#include "AStarInteruptable.h"

#include <SDL.h>
#include <fstream>
#include <vector>

const int GRID_SQUARE_SIZE = 32;
const Uint32 MAX_UNITS = 100;
const std::string gFileName = "pathgrid.txt";

GameApp::GameApp()
	:mpMessageManager(NULL)
	, mpGrid(NULL)
	, mpGridGraph(NULL)
{
}

GameApp::~GameApp()
{
	cleanup();
}

bool GameApp::init()
{
	bool retVal = Game::init();
	if (retVal == false)
	{

		return false;
	}

	mpMessageManager = new GameMessageManager();
	mpInputSystem = new InputSystem();
	//create and load the Grid, GridBuffer, and GridRenderer
	mpGrid = new Grid(mpGraphicsSystem->getWidth(), mpGraphicsSystem->getHeight(), GRID_SQUARE_SIZE);
	mpGridVisualizer = new GridVisualizer(mpGrid);
	std::ifstream theStream(gFileName);
	mpGrid->load(theStream);

	//create the GridGraph for pathfinding
	mpGridGraph = new GridGraph(mpGrid);
	//init the nodes and connections
	mpGridGraph->init();

	//pathFinder DepthFirstPathfinder DijkstraPathfinder
	mpPathfinder = new DijkstraPathfinder(mpGridGraph);
	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);

	mpComponentManager = new ComponentManager(MAX_UNITS);
	mpUnitManager = new UnitManager(MAX_UNITS);

	mpDataLoader = new DataLoader();
	mpDataLoader->loadData();

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, "wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID,"arrow.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID,"enemy-arrow.png");
	mpGraphicsBufferManager->loadBuffer(mTargetBufferID,"target.png");

	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer(mBackgroundBufferID);
	if (pBackGroundBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight());
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer( mPlayerIconBufferID );
	Sprite* pArrowSprite = NULL;
	if( pPlayerBuffer != NULL )
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite( PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, (float)pPlayerBuffer->getWidth(), (float)pPlayerBuffer->getHeight() );
	}
	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	Sprite* pEnemyArrow = NULL;
	if (pAIBuffer != NULL)
	{
		pEnemyArrow = mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, (float)pAIBuffer->getWidth(), (float)pAIBuffer->getHeight());
	}

	GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);
	if (pTargetBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, (float)pTargetBuffer->getWidth(), (float)pTargetBuffer->getHeight());
	}

	mpMasterTimer->start();
	return true;
}

void GameApp::cleanup()
{
	delete mpMessageManager;
	mpMessageManager = NULL;

	delete mpInputSystem;
	mpInputSystem = nullptr;

	delete mpGrid;
	mpGrid = NULL;

	delete mpGridVisualizer;
	mpGridVisualizer = NULL;

	delete mpGridGraph;
	mpGridGraph = NULL;

	delete mpPathfinder;
	mpPathfinder = NULL;

	delete mpDebugDisplay;
	mpDebugDisplay = NULL;

	delete mpDataLoader;
	mpDataLoader = NULL;

	delete mpUnitManager;
	mpUnitManager = NULL;

	delete mpComponentManager;
	mpComponentManager = NULL;
}

void GameApp::beginLoop()
{
	//should be the first thing done
	Game::beginLoop();
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;

void GameApp::processLoop()
{
	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);

	//get back buffer
	GraphicsBuffer* pBackBuffer = mpGraphicsSystem->getBackBuffer();
	//copy to back buffer
	mpGridVisualizer->draw(*pBackBuffer);
#ifdef VISUALIZE_PATH
	//show pathfinder visualizer
	mpPathfinder->drawVisualization(mpGrid, pBackBuffer);
#endif

	mpDebugDisplay->draw(pBackBuffer);

	mpMessageManager->processMessagesForThisframe();


	mpInputSystem->update();

	//draw units
	mpUnitManager->drawAll();

	//should be last thing in processLoop
	Game::processLoop();
}

bool GameApp::endLoop()
{
	return Game::endLoop();
}

void GameApp::changeToDijkstra()
{
	ClearPathMap();
	delete mpPathfinder;
	delete mpDebugDisplay;
	mpPathfinder = new DijkstraPathfinder(mpGridGraph);
	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
	pathMap.clear();
}

void GameApp::changeToDFS()
{
	ClearPathMap();
	delete mpPathfinder;
	delete mpDebugDisplay;
	mpPathfinder = new DepthFirstPathfinder(mpGridGraph);
	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
}

void GameApp::changeToAStar()
{
	ClearPathMap();
	delete mpPathfinder;
	delete mpDebugDisplay;
	mpPathfinder = new AStarPathfinder(mpGridGraph);
	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
}



void GameApp::ClearPathMap(){
	pathMap.erase(pathMap.begin(), pathMap.end());
}

void GameApp::adjustFlockUI()
{
	float xPosit = 20;
	float yPosit = 500;
	float yIncrement = -30;

	std::string toWrite = "R -- Cohesion = ";
	toWrite += truncateFloat(mpDataLoader->getCohesionFactor());
	toWrite += " ++ T";
	mpGraphicsSystem->writeText(*mpFont, xPosit, yPosit, toWrite, BLACK_COLOR);

	yPosit -= yIncrement;
	toWrite = "F -- Separation = ";
	toWrite += truncateFloat(mpDataLoader->getSeparationFactor());
	toWrite+= " ++ G";
	mpGraphicsSystem->writeText(*mpFont, xPosit, yPosit, toWrite, BLACK_COLOR);

	yPosit -= yIncrement;
	toWrite = "V -- Alignment = ";
	toWrite += truncateFloat(mpDataLoader->getAlignmentFactor());
	toWrite +=  " ++ B";
	mpGraphicsSystem->writeText(*mpFont, xPosit, yPosit, toWrite, BLACK_COLOR);

	yPosit -= yIncrement;
	toWrite = "Y -- Wander = ";
	toWrite += truncateFloat(mpDataLoader->getWanderFactor());
	toWrite += " ++ U";
	mpGraphicsSystem->writeText(*mpFont, xPosit, yPosit, toWrite, BLACK_COLOR);

	yPosit -= yIncrement;
	toWrite = "H -- Cohesion = ";
	toWrite += truncateFloat(mpDataLoader->getCohesionRadius());
	toWrite += " ++ J";
	mpGraphicsSystem->writeText(*mpFont, xPosit, yPosit, toWrite, BLACK_COLOR);

	yPosit -= yIncrement;
	toWrite = "N -- Alignment = ";
	toWrite += truncateFloat(mpDataLoader->getAlignmentRadius());
	toWrite += " ++ M";
	mpGraphicsSystem->writeText(*mpFont, xPosit, yPosit, toWrite, BLACK_COLOR);

	yPosit -= yIncrement;
	toWrite = "I -- Separation = ";
	toWrite += truncateFloat(mpDataLoader->getSeparationRadius());
	toWrite += " ++ O";
	mpGraphicsSystem->writeText(*mpFont, xPosit, yPosit, toWrite, BLACK_COLOR);

	yPosit -= yIncrement;
	toWrite = "";
	mpGraphicsSystem->writeText(*mpFont, xPosit, yPosit, toWrite, BLACK_COLOR);
}

std::string GameApp::truncateFloat(float num)
{ //truncate to two decimal places and return;
	std::string str = std::to_string(num);
	return str.substr(0, str.length() - 4);
}

void GameApp::UpdateSteering(int index, Path* path)
{
	std::vector<Unit*> units = mpUnitManager->getAllUnits();
	Unit* unit = units[index];
	if(!unit)
	{
		return;
	}
	std::vector<Vector2D> nodePositions;
	for(int c = 0; c < path->getNumNodes(); c++)
	{
		nodePositions.push_back(mpGrid->getULCornerOfSquare(path->peekNode(c)->getId()));
	}
	unit->setSteering(Steering::ARRIVETOALLSTEERING, nodePositions);

}

void GameApp::MakeUnits(){
	for(int i = 0; i < UNIT_SIZE; i++)
	{
		mpUnitManager->deleteUnit(i + 1);
		Unit* pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(1));
		pUnit->setSteering(Steering::FACE, ZERO_VECTOR2D);
	}
}

void GameApp::CachePath(Node* n1, Node* n2, Path* path)
{
	std::string name = std::to_string(n1->getId()) + std::to_string(n2->getId());
	pathMap.insert(std::make_pair(name, path));
}

Path* GameApp::FindPath(Node* n1, Node* n2)
{
	std::string name = std::to_string(n1->getId()) + std::to_string(n2->getId());
	if(pathMap.find(name) != pathMap.end())
	{
		return pathMap[name];
	}
	else 
	{
		return NULL;
	}
}