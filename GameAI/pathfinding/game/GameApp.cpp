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
#include "FlowFieldPathfinder.h"
#include "PathfindManager.h"
#include "StateMachine.h"
#include "WanderState.h"
#include "MoveTowardsState.h"
#include "CandyState.h"
#include "RunAwayState.h"
#include "Spawner.h"
#include <SDL.h>
#include <fstream>
#include <vector>
#include "SoundManager.h"

const int GRID_SQUARE_SIZE = 32;
const int NUM_ENEMIES = 3;
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
	mScore = 0;
	bool retVal = Game::init();
	if (retVal == false)
	{

		return false;
	}

	isFlow = false;
	mGameOver = false;

	mpSoundManager = new SoundManager();
	mpSoundManager->init();
	mpSoundManager->loadFiles();

	mpPathfindManager = new PathfindManager();
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
	mpPathfinder = new AStarPathfinder(mpGridGraph);
	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);

	mpComponentManager = new ComponentManager(MAX_UNITS);
	mpUnitManager = new UnitManager(MAX_UNITS);

	mpDataLoader = new DataLoader();
	mpDataLoader->loadData();
	mpSpawner = new Spawner();
	mpSpawner->init();

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, "wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID,"snakey.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID,"enemy_sprite.png");
	mpGraphicsBufferManager->loadBuffer(mTargetBufferID,"target.png");

	//need to delete these?
	mpGraphicsBufferManager->loadBuffer(mEnemyFoodBufferID, "food1_sprite.png");
	mpGraphicsBufferManager->loadBuffer(mCoinBufferID, "coin_sprite.png");
	mpGraphicsBufferManager->loadBuffer(mMightyCandyBufferID, "food4_sprite.png");
	mpGraphicsBufferManager->loadBuffer(mPoweredPlayerBufferID, "snakey_angry.png");
	mpGraphicsBufferManager->loadBuffer(mScaredEnemyBufferID, "AI_scared_sprites.png");


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

	//need to delete these?
	GraphicsBuffer* pEnemyFoodBuffer = mpGraphicsBufferManager->getBuffer(mEnemyFoodBufferID);
	if (pEnemyFoodBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(ENEMY_FOOD_SPRITE_ID, pEnemyFoodBuffer, 0, 0, (float)pEnemyFoodBuffer->getWidth(), (float)pEnemyFoodBuffer->getHeight());
	}
	GraphicsBuffer* pCoinBuffer = mpGraphicsBufferManager->getBuffer(mCoinBufferID);
	if (pCoinBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(COIN_SPRITE_ID, pCoinBuffer, 0, 0, (float)pCoinBuffer->getWidth(), (float)pCoinBuffer->getHeight());
	}
	GraphicsBuffer* pMightyCandyBuffer = mpGraphicsBufferManager->getBuffer(mMightyCandyBufferID);
	if (pMightyCandyBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(MIGHTY_CANDY_SPRITE_ID, pMightyCandyBuffer, 0, 0, (float)pMightyCandyBuffer->getWidth(), (float)pMightyCandyBuffer->getHeight());
	}
	GraphicsBuffer* pPoweredPlayerBuffer = mpGraphicsBufferManager->getBuffer(mPoweredPlayerBufferID);
	Sprite* pPoweredPlayerSprite = NULL;
	if (pPoweredPlayerBuffer != NULL)
	{
		pPoweredPlayerSprite = mpSpriteManager->createAndManageSprite(POWERED_PLAYER_SPRITE_ID, pPoweredPlayerBuffer, 0, 0, (float)pPoweredPlayerBuffer->getWidth(), (float)pPoweredPlayerBuffer->getHeight());
	}
	GraphicsBuffer* pScaredEnemyBuffer = mpGraphicsBufferManager->getBuffer(mScaredEnemyBufferID);
	Sprite* pScaredEnemySprite = NULL;
	if (pScaredEnemyBuffer != NULL)
	{
		pScaredEnemySprite = mpSpriteManager->createAndManageSprite(SCARED_ENEMY_SPRITE_ID, pScaredEnemyBuffer, 0, 0, (float)pScaredEnemyBuffer->getWidth(), (float)pScaredEnemyBuffer->getHeight());
	}
	mpSpawner->spawnMightyCandies();

	PositionData posData;
	posData.pos = Vector2D(50, 50);

	//Add the state machine to the player incase you want to use it
	Unit* player = mpUnitManager->createPlayerUnit(*pArrowSprite, true, posData);
	player->setAltSprite(pPoweredPlayerSprite);
	player->setType(Unit::PLAYER);
	StateMachineState* wanderState = new WanderState(0, true, player->GetID());
	StateMachineState* moveTowardsState = new MoveTowardsState(1, true, player->GetID());
	StateMachineState* candyState = new CandyState(2, true, player->GetID());
	StateMachineState* runAwayState = new RunAwayState(3, true, player->GetID());

	StateTransition* pTowardsTransition = new StateTransition(TOWARDS_TRANSITION, 1);
	StateTransition* pCandyTransition = new StateTransition(CANDY_TRANSITION, 2);
	StateTransition* pRunAwayTransition = new StateTransition(RUNAWAY_TRANSITION, 3);
	StateTransition* pWanderTransition = new StateTransition(WANDER_TRANSITION, 0);

	wanderState->addTransition(pTowardsTransition);
	wanderState->addTransition(pRunAwayTransition);
	wanderState->addTransition(pCandyTransition);
	moveTowardsState->addTransition(pWanderTransition);
	moveTowardsState->addTransition(pRunAwayTransition);
	candyState->addTransition(pWanderTransition);
	runAwayState->addTransition(pWanderTransition);
	runAwayState->addTransition(pTowardsTransition);

	player->getPositionComponent()->setScreenWrap(false);
	player->getStateMachine()->addState(wanderState);
	player->getStateMachine()->addState(moveTowardsState);
	player->getStateMachine()->addState(candyState);
	player->getStateMachine()->addState(runAwayState);
	player->getStateMachine()->setInitialStateID(0);

	for(int i = 0; i < NUM_ENEMIES; i++){
		Unit* pUnit = makeEnemyUnit();
	}
	mpMasterTimer->start();
	mpSoundManager->playMusic();
	return true;
}

void GameApp::cleanup()
{
	mpSoundManager->cleanup();
	delete mpSoundManager;
	mpSoundManager = NULL;

	delete mpMessageManager;
	mpMessageManager = NULL;

	delete mpInputSystem;
	mpInputSystem = nullptr;

	delete mpPathfindManager;
	mpPathfindManager = nullptr;

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

	delete mpSpawner;
	mpSpawner = NULL;
}

void GameApp::beginLoop()
{
	//should be the first thing done
	Game::beginLoop();
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;

void GameApp::processLoop()
{
	//get back buffer
	GraphicsBuffer* pBackBuffer = mpGraphicsSystem->getBackBuffer();
	mpInputSystem->update();
	mpMessageManager->processMessagesForThisframe();
	if(mGameOver){
		mpGraphicsSystem->writeText(*pBackBuffer, *getFont(), 500, 500, "GAME OVER! Esc to quit", BLACK_COLOR);
	}
	else {
		//spawn before collisions in case a coin spawns under the player
		mpSpawner->update(TARGET_ELAPSED_MS);
		mpUnitManager->updateAll(TARGET_ELAPSED_MS);
		mpUnitManager->runCollisions();
		mpComponentManager->update(TARGET_ELAPSED_MS);
		
		//Process pathfinding for the frame.
		mpPathfindManager->update(LOOP_TARGET_TIME / 2.0f);

		//copy to back buffer
		mpGridVisualizer->draw(*pBackBuffer);
#ifdef VISUALIZE_PATH
		//show pathfinder visualizer
		mpPathfinder->drawVisualization(mpGrid, pBackBuffer);
#endif

		mpDebugDisplay->draw(pBackBuffer);

		//draw units
		mpUnitManager->drawAll();
		mpGraphicsSystem->writeText(*pBackBuffer, *getFont(), 100, 50, "Player Health: " + std::to_string(mpUnitManager->getPlayerUnit()->getHealth()), BLACK_COLOR);
		mpGraphicsSystem->writeText(*pBackBuffer, *getFont(), 100, 100, "Player Score: " + std::to_string(getScore()), BLACK_COLOR);
	}
	
	//should be last thing in processLoop
	Game::processLoop();
}

bool GameApp::endLoop()
{
	return Game::endLoop();
}

void GameApp::changeToDijkstra()
{
	isFlow = false;
	ClearPathMap();
	delete mpPathfinder;
	delete mpDebugDisplay;
	mpPathfinder = new DijkstraPathfinder(mpGridGraph);
	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
}

void GameApp::changeToDFS()
{
	isFlow = false;
	ClearPathMap();
	delete mpPathfinder;
	delete mpDebugDisplay;
	mpPathfinder = new DepthFirstPathfinder(mpGridGraph);
	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
}

void GameApp::changeToAStar()
{
	isFlow = false;
	ClearPathMap();
	delete mpPathfinder;
	delete mpDebugDisplay;
	mpPathfinder = new AStarPathfinder(mpGridGraph);
	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
}

void GameApp::changeToFlow()
{
	isFlow = true;
	ClearPathMap();
	delete mpPathfinder;
	delete mpDebugDisplay;
	mpPathfinder = new FlowFieldPathfinder(mpGridGraph);
	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
}
void GameApp::changeToInteruptable()
{
	ClearPathMap();
	delete mpPathfinder;
	delete mpDebugDisplay;
	mpPathfinder = new AStarInteruptable(mpGridGraph);
	PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
	mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
}

void GameApp::ClearPathMap(){
	pathMap.erase(pathMap.begin(), pathMap.end());
}

std::string GameApp::truncateFloat(float num)
{ //truncate to two decimal places and return;
	std::string str = std::to_string(num);
	return str.substr(0, str.length() - 4);
}

void GameApp::UpdateSteering(int index, Path* path)
{
	std::vector<Unit*> units = mpUnitManager->getAllUnits();
	if (isFlow)
	{
		for (int i = 0; i < UNIT_SIZE; i++)
		{
			Unit* unit = units[i];
			unit->setSteering(Steering::FLOW);
		}
	}
	else
	{
		Unit* unit = units[index];
		if (!unit)
		{
			return;
		}
		std::vector<Vector2D> nodePositions;
		for (int c = 0; c < path->getNumNodes(); c++)
		{
			nodePositions.push_back(mpGrid->getULCornerOfSquare(path->peekNode(c)->getId()));
		}
		unit->setSteering(Steering::ARRIVETOALLSTEERING, nodePositions);
	}
}

void GameApp::MakeUnits(){

	std::vector<Unit*> units = mpUnitManager->getAllUnits();
	Unit* player = mpUnitManager->getPlayerUnit();
	for(int i = 0; i < units.size(); i++){
		if(player != units[i]){
			mpUnitManager->deleteUnit(units[i]->GetID());
		}
	}

	for(int i = 0; i < UNIT_SIZE; i++)
	{
		Unit* pUnit = mpUnitManager->CreateRandomUnitNoWall(*mpSpriteManager->getSprite(1), mpGridGraph);
		pUnit->setSteering(Steering::FACE, ZERO_VECTOR2D);
		pUnit->getPositionComponent()->setScreenWrap(false);
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

void GameApp::generatePath(Node * pToNode, Node * pFromNode, int mIdNum)
{
	mpPathfindManager->addPathToFind(pFromNode, pToNode, mIdNum);
}

Unit * GameApp::makeEnemyUnit()
{
	Unit* pUnit = mpUnitManager->CreateRandomUnitNoWall(*mpSpriteManager->getSprite(AI_ICON_SPRITE_ID), mpGridGraph);
	pUnit->setAltSprite(mpSpriteManager->getSprite(SCARED_ENEMY_SPRITE_ID));
	pUnit->setType(Unit::ENEMY);
	StateMachineState* wanderState = new WanderState(0, false, pUnit->GetID());
	StateMachineState* moveTowardsState = new MoveTowardsState(1, false, pUnit->GetID());
	StateMachineState* candyState = new CandyState(2, false, pUnit->GetID());
	StateMachineState* runAwayState = new RunAwayState(3, false, pUnit->GetID());

	StateTransition* pTowardsTransition = new StateTransition(TOWARDS_TRANSITION, 1);
	StateTransition* pCandyTransition = new StateTransition(CANDY_TRANSITION, 2);
	StateTransition* pRunAwayTransition = new StateTransition(RUNAWAY_TRANSITION, 3);
	StateTransition* pWanderTransition = new StateTransition(WANDER_TRANSITION, 0);

	wanderState->addTransition(pTowardsTransition);
	wanderState->addTransition(pRunAwayTransition);
	wanderState->addTransition(pCandyTransition);
	moveTowardsState->addTransition(pWanderTransition);
	moveTowardsState->addTransition(pRunAwayTransition);
	candyState->addTransition(pWanderTransition);
	runAwayState->addTransition(pWanderTransition);
	runAwayState->addTransition(pTowardsTransition);

	pUnit->setSteering(Steering::WANDERPATH, ZERO_VECTOR2D);
	pUnit->getPositionComponent()->setScreenWrap(false);
	pUnit->getStateMachine()->addState(wanderState);
	pUnit->getStateMachine()->addState(moveTowardsState);
	pUnit->getStateMachine()->addState(candyState);
	pUnit->getStateMachine()->addState(runAwayState);
	pUnit->getStateMachine()->setInitialStateID(0);
	return pUnit;
}
