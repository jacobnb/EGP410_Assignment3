#pragma once
#include <Trackable.h>
#include <vector>
class DataLoader : public Trackable
{
public:
	DataLoader();
	~DataLoader();
	enum DATA_KEY {
		HIGH_SCORE,
		COIN_SPAWN_PER,
		CANDY_SPAWN_PER,
		ENEMY_SPAWN_TIME,
		ENEMY_HP,
		ENEMY_FOOD_TIME,
		ENEMY_DAMAGE,
		PLAYER_DAMAGE,
		PLAYER_VELOCITY,
		MIGHTY_CANDY_TIME,
		ENEMY_VELOCITY,
	};
	void loadData();
	void writeData();
	float getData(DATA_KEY type);
	
private:
	const std::string mFilePath = "data.txt";
	float stringToFloat(std::string str);
	std::vector<float> data;
};