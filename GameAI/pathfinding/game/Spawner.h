#pragma once
#include <Trackable.h>
#include "Timer.h"
#include <vector>
#include "Vector2D.h"
class Spawner : public Trackable
{
public:
	Spawner();
	~Spawner();
	void init();
	void update(float lastFrameTime);

private:
	//cache the spawn points from Grid
	void setSpawnPoints();

	std::vector<Vector2D> mEnemySpawnPoints; // these need to spawn every x seconds.
	std::vector<Vector2D> mMightyCandySpawnPoints; //these need to respawn after x seconds.
};