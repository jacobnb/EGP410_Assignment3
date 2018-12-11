#pragma once
#include <Trackable.h>
#include "Timer.h"
#include <vector>
#include "Vector2D.h"
//Spawner handles spawning of enemies, coins, enemy candy, and mighty candy
class Spawner : public Trackable
{
public:
	Spawner();
	~Spawner();
	void init(); //cache spawn points
	void update(float lastFrameTime); //decrement timers and spawn units.
	void spawnMightyCandies();

	void spawnCoin();
	void spawnEnemyFood();
	void spawnEnemy();
private:
	//cache the spawn points from Grid
	void setSpawnPoints();
	Vector2D getEmptyPoint();
	float coinSpawnTimer;
	float enemyFoodSpawnTimer;
	float enemySpawnTimer;
	std::vector<Vector2D> mEnemySpawnPoints; // these need to spawn every x seconds.
	std::vector<Vector2D> mMightyCandySpawnPoints; //these need to respawn after x seconds.
};