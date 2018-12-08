#include "Spawner.h"
#include "Game.h"
#include "Grid.h"
#include "GameApp.h"
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
