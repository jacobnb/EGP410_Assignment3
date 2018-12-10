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

void Spawner::spawnCoin()
{
	//TODO
	auto pUnit = gpGame->getUnitManager()->createUnit(*gpGame->getSpriteManager()->getSprite(COIN_SPRITE_ID));
	pUnit->getPositionComponent()->setPosition(getEmptyPoint());
	pUnit->setType(Unit::COIN);

}

void Spawner::spawnEnemyFood()
{
	//TODO
	auto pUnit = gpGame->getUnitManager()->createUnit(*gpGame->getSpriteManager()->getSprite(ENEMY_FOOD_SPRITE_ID));
	pUnit->getPositionComponent()->setPosition(getEmptyPoint());
	pUnit->setType(Unit::ENEMY_FOOD);
}

void Spawner::spawnEnemy()
{
	//TODO
	auto pUnit = gpGame->getUnitManager()->createUnit(*gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID));
	//move to valid spawn point.
	int index = 0; 
	//TODO get random index or index far from player.
	pUnit->getPositionComponent()->setPosition(mEnemySpawnPoints[index]);
	pUnit->setType(Unit::ENEMY);

}

void Spawner::setSpawnPoints()
{
	auto grid = static_cast<GameApp*>(gpGame)->getGrid();
	int length = grid->getGridHeight() * grid->getGridWidth();
	for (int i = 0; i <= length; i++) { // should be less than length
		if (grid->getValueAtIndex(i) == ENEMY_SPAWN) {
			mEnemySpawnPoints.push_back(grid->getULCornerOfSquare(i));
		}
		else if (grid->getValueAtIndex(i) == MIGHTY_CANDY) {
			mMightyCandySpawnPoints.push_back(grid->getULCornerOfSquare(i));
		}
	}
}

Vector2D Spawner::getEmptyPoint()
{
	return ZERO_VECTOR2D;
}
