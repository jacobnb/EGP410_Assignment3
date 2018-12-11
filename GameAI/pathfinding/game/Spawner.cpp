#include "Spawner.h"
#include "Game.h"
#include "Grid.h"
#include "GameApp.h"
#include "DataLoader.h"
#include "UnitManager.h"
#include "SpriteManager.h"
#include "Unit.h"
#include "Vector2D.h"
Spawner::Spawner()
{
}

Spawner::~Spawner()
{
}

void Spawner::init()
{
	setSpawnPoints();
}


void Spawner::update(float lastFrameTime)
{
	coinSpawnTimer -= lastFrameTime;
	enemyFoodSpawnTimer -= lastFrameTime;
	enemySpawnTimer -= lastFrameTime;

	if (coinSpawnTimer < 0) {
		spawnCoin();
		coinSpawnTimer = gpGame->getDataLoader()->getData(DataLoader::COIN_SPAWN_TIME);
	}
	if (enemyFoodSpawnTimer < 0) {
		spawnEnemyFood();
		enemyFoodSpawnTimer = gpGame->getDataLoader()->getData(DataLoader::ENEMY_FOOD_TIME);
	}
	if (enemySpawnTimer < 0) {
		spawnEnemy();
		enemySpawnTimer = gpGame->getDataLoader()->getData(DataLoader::ENEMY_SPAWN_TIME);
	}
}

void Spawner::spawnMightyCandies()
{
	//TODO
}

void Spawner::spawnCoin()
{
	auto pUnit = gpGame->getUnitManager()->createUnit(*gpGame->getSpriteManager()->getSprite(COIN_SPRITE_ID));
	pUnit->getPositionComponent()->setPosition(getEmptyPoint());
	pUnit->setType(Unit::COIN);
}

void Spawner::spawnEnemyFood()
{
	auto pUnit = gpGame->getUnitManager()->createUnit(*gpGame->getSpriteManager()->getSprite(ENEMY_FOOD_SPRITE_ID));
	pUnit->getPositionComponent()->setPosition(getEmptyPoint());
	pUnit->setType(Unit::ENEMY_FOOD);
}

void Spawner::spawnEnemy()
{
	auto pUnit = static_cast<GameApp*>(gpGame)->makeEnemyUnit();
	//move to valid spawn point.
	int index = 1; 
	//TODO get random index or index far from player.
	pUnit->getPositionComponent()->setPosition(mEnemySpawnPoints[index]);
	pUnit->setType(Unit::ENEMY);
}

//==TODO add spawn offset so they spawn in the middle of the square==//
void Spawner::setSpawnPoints()
{
	auto grid = static_cast<GameApp*>(gpGame)->getGrid();
	int length = grid->getGridHeight() * grid->getGridWidth();
	for (int i = 0; i <= length; i++) { // should probably be less than length
		if (grid->getValueAtIndex(i) == ENEMY_SPAWN) {
			mEnemySpawnPoints.push_back(grid->getULCornerOfSquare(i));
		}
		else if (grid->getValueAtIndex(i) == MIGHTY_CANDY) {
			mMightyCandySpawnPoints.push_back(grid->getULCornerOfSquare(i));
		}
	}
}

#include <stdlib.h>
Vector2D Spawner::getEmptyPoint()
{	
	auto grid = static_cast<GameApp*>(gpGame)->getGrid();
	int length = grid->getGridHeight() * grid->getGridWidth();
	int index = rand() % length;
	while (grid->getValueAtIndex(index) != CLEAR_VALUE) {
		index = rand() % length;
	}
	return grid->getULCornerOfSquare(index);
}
