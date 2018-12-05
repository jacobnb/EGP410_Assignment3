#include "DataLoader.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "SteeringComponent.h"

DataLoader::DataLoader()
{
}

DataLoader::~DataLoader()
{
}

void DataLoader::loadData()
{
	std::ifstream dataStream;
	dataStream.open(mFilePath, std::ifstream::in);
	assert(dataStream.is_open());

	std::string str;
	data.clear();
	while (!dataStream.eof()) {
		//==read data into vars==//
		std::getline(dataStream, str, ':');
		std::getline(dataStream, str, ',');
		data.push_back( stringToFloat(str) );
	}
	data.shrink_to_fit();
}


void DataLoader::writeData()
{
	
	std::ofstream dataStream;
	dataStream.open(mFilePath);
	assert(dataStream.is_open());
	dataStream << "High Score: " << data[HIGH_SCORE] << ",\n";
	dataStream << "Coin spawn percentage: " << data[COIN_SPAWN_PER] << ",\n";
	dataStream << "Candy spawn percentage: " << data[CANDY_SPAWN_PER] << ",\n";
	dataStream << "Enemy spawn time: "<< data[ENEMY_SPAWN_TIME] << ",\n";
	dataStream << "Enemy hp : "<< data[ENEMY_HP] << ", \n";
	dataStream << "Enemy powerup spawn time : "<< data[ENEMY_FOOD_TIME] << ", \n";
	dataStream << "Enemy damage dealt to player : "<< data[ENEMY_DAMAGE] << ", \n";
	dataStream << "player damage dealt to enemy : "<< data[PLAYER_DAMAGE] << ", \n";
	dataStream << "Player velocity : "<< data[PLAYER_VELOCITY] << ", \n";
	dataStream << "Mighty candy spawn time(60 seconds) : "<< data[MIGHTY_CANDY_TIME] << ", \n";
	dataStream << "enemy velocity as % of players(80 % ) : "<< data[ENEMY_VELOCITY] << ", \n";
	dataStream.close();
}

float DataLoader::getData(DATA_KEY type)
{
	if (type == ENEMY_VELOCITY) {
		// enemy velocity is defined as a % of players velocity.
		return ENEMY_VELOCITY * PLAYER_VELOCITY;
	}
	return data[type];
}



float DataLoader::stringToFloat(std::string str)
{
	float num = 0;
	try
		//https://stackoverflow.com/questions/18534036/checking-the-int-limits-in-stoi-function-in-c
	{
		num = std::stof(str);
	}
	catch (std::invalid_argument& e)
	{
		e; //deals with compiler warning
		std::cout << "Can't convert "<< str << "to a float";
		return false;
	}
	catch (std::out_of_range& e)
	{
		e;//deals with compiler warning
		std::cout << "Error in data loading";
		return false;
	}
	return num;
}
