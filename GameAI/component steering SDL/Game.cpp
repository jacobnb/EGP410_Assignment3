#include <stdio.h>
#include <assert.h>

#include <sstream>
#include <SDL.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Font.h"
#include "GraphicsBufferManager.h"
#include "GameMessageManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Timer.h"
#include "InputSystem.h"
#include "PlayerMoveToMessage.h"
#include "ComponentManager.h"
#include "UnitManager.h"
#include "DataLoader.h"
#include "FlockingSteering.h"
Game* gpGame = NULL;

const int WIDTH = 1024;
const int HEIGHT = 768;
const Uint32 MAX_UNITS = 100;

Game::Game()
	:mpGraphicsSystem(NULL)
	, mpGraphicsBufferManager(NULL)
	, mpSpriteManager(NULL)
	, mpLoopTimer(NULL)
	, mpMasterTimer(NULL)
	, mpFont(NULL)
	, mShouldExit(false)
	, mBackgroundBufferID("")
	, mpMessageManager(NULL)
	, mpComponentManager(NULL)
	, mpUnitManager(NULL)
	, mpInputSystem(NULL)
	, mpDataLoader(NULL)
{
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	//create and init GraphicsSystem
	mpGraphicsSystem = new GraphicsSystem();
	bool goodGraphics = mpGraphicsSystem->init( WIDTH, HEIGHT );
	if(!goodGraphics) 
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager(mpGraphicsSystem);
	mpSpriteManager = new SpriteManager();

	mpDataLoader = new DataLoader();
	mpDataLoader->loadData();

	mpMessageManager = new GameMessageManager();
	mpInputSystem = new InputSystem();
	mpComponentManager = new ComponentManager(MAX_UNITS);
	mpUnitManager = new UnitManager(MAX_UNITS);

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID,"wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID,"arrow.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID,"enemy-arrow.png");
	mpGraphicsBufferManager->loadBuffer(mTargetBufferID,"target.png");

	//load Font
	mpFont = new Font("cour.ttf", 24);
	
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
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

	//setup units
	Unit* pUnit = mpUnitManager->createPlayerUnit(*pArrowSprite);
	//pUnit->setShowTarget(true);
	pUnit->setSteering(Steering::WANDER, ZERO_VECTOR2D);

	return true;
}

void Game::cleanup()
{

	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	delete mpFont;
	mpFont = NULL;

	//delete the graphics system
	delete mpGraphicsSystem;
	mpGraphicsSystem = NULL;

	delete mpDataLoader;
	mpDataLoader = NULL;
	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;
	delete mpMessageManager;
	mpMessageManager = NULL;
	delete mpUnitManager;
	mpUnitManager = NULL;
	delete mpComponentManager;
	mpComponentManager = NULL;
	delete mpInputSystem;
	mpInputSystem = NULL;
}

void Game::beginLoop()
{
	mpLoopTimer->start();
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;
	
void Game::processLoop()
{
	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);
	
	//draw background
	Sprite* pBackgroundSprite = mpSpriteManager->getSprite( BACKGROUND_SPRITE_ID );
	GraphicsBuffer* pDest = mpGraphicsSystem->getBackBuffer();
	mpGraphicsSystem->draw(*pDest, *pBackgroundSprite, 0.0f, 0.0f);

	//draw units
	mpUnitManager->drawAll();
	
	//draw flock UI
	adjustFlockUI();
	
	mpInputSystem->update();
	//test of fill region
	mpGraphicsSystem->fillRegion(*pDest, Vector2D(300, 300), Vector2D(500, 500), RED_COLOR);
	mpGraphicsSystem->swap();
	mpInputSystem->updateAll();
	mpMessageManager->processMessagesForThisframe();

}

bool Game::endLoop()
{
	mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( LOOP_TARGET_TIME );
	return mShouldExit;
}

void Game::spawnEnemyAtRandomLoc()
{
	Unit* pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(AI_ICON_SPRITE_ID));
	if (pUnit == NULL) { //prevents overflow error.
		deleteRandomEnemyUnit();
	}
	else {
		pUnit->setSteering(Steering::WANDER_CHASE, ZERO_VECTOR2D, PLAYER_UNIT_ID);
		//pUnit->setShowTarget(true);
		pUnit->getPositionComponent()->setScreenWrap(true);
	}
	
}

void Game::deleteRandomEnemyUnit()
{
	mpUnitManager->deleteRandomUnit();
}

void Game::spawnTenBoids()
{
	for (int c = 0; c < 10; c++) {
		Unit* pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(AI_ICON_SPRITE_ID));
		if (pUnit == NULL) { //prevents overflow error.
			deleteRandomEnemyUnit();
		}
		else {
			pUnit->setSteering(Steering::FLOCK, ZERO_VECTOR2D, PLAYER_UNIT_ID);
			//pUnit->setShowTarget(true);
			pUnit->getPositionComponent()->setScreenWrap(true);
		}
	}
}

void Game::adjustFlockUI()
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

std::string Game::truncateFloat(float num)
{ //truncate to two decimal places and return;
	std::string str = std::to_string(num);
	return str.substr(0, str.length() - 4);
}


float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
}

