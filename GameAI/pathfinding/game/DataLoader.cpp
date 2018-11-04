#include "DataLoader.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include "FlockMessage.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "FlockingSteering.h"
#include "SteeringComponent.h"

void DataLoader::loadData()
{
	std::ifstream dataStream;
	dataStream.open(mFilePath, std::ifstream::in);
	assert(dataStream.is_open());

	std::string str;

	//==read data into vars==//
	std::getline(dataStream, str, ':');
	std::getline(dataStream, str, ',');
	mCohesionFactor = stringToFloat(str);
	std::getline(dataStream, str, ':');
	std::getline(dataStream, str, ',');
	mSeparationFactor = stringToFloat(str);
	std::getline(dataStream, str, ':');
	std::getline(dataStream, str, ',');
	mAlignmentFactor = stringToFloat(str);
	std::getline(dataStream, str, ':');
	std::getline(dataStream, str, ',');
	mWanderFactor = stringToFloat(str);
	std::getline(dataStream, str, ':');
	std::getline(dataStream, str, ',');
	mCohesionRadius = stringToFloat(str);
	std::getline(dataStream, str, ':');
	std::getline(dataStream, str, ',');
	mAlignmentRadius= stringToFloat(str);
	std::getline(dataStream, str, ':');
	std::getline(dataStream, str, ',');
	mSeparationRadius= stringToFloat(str);
}


void DataLoader::writeData()
{
	updateFlockingData();
	
	std::ofstream dataStream;
	dataStream.open(mFilePath);
	assert(dataStream.is_open());
	dataStream << "{\n";
	dataStream << "\"Cohesion Factor\": " << mCohesionFactor << ",\n";
	dataStream << "\"Separation Factor\": " << mSeparationFactor << ",\n";
	dataStream << "\"Alignment Factor\": " << mAlignmentFactor << ",\n";
	dataStream << "\"Wander Factor\": " << mWanderFactor << ",\n";
	dataStream << "\"Cohesion Radius\": " << mCohesionRadius << ",\n";
	dataStream << "\"Alignment Radius\": " << mAlignmentRadius << ",\n";
	dataStream << "\"Separation Radius\": " << mSeparationRadius << ",\n";
	dataStream << "}";
	dataStream.close();
}

void DataLoader::updateFlockingData()
{
	std::vector<Unit*>::iterator iter;
	std::vector<Unit*> units = gpGame->getUnitManager()->getAllUnits();
	iter = units.begin();
	while ((*iter)->getSteeringComponent()->getType() != Steering::FLOCK) {
		iter++;
		if (iter == units.end()) {
			return;
			//if no valid units found then write nothing.
		}
	}
	//All flocking steerings should have the same values, so only need the first one.
	FlockingSteering* flockSteering = (*iter)->getSteeringComponent()->getFlockingSteering();
	assert(flockSteering);

	mCohesionFactor = flockSteering->getCohesionFactor();
	mSeparationFactor = flockSteering->getSeparationFactor();
	mAlignmentFactor = flockSteering->getAlignmentFactor();
	mWanderFactor = flockSteering->getWanderFactor();
	mCohesionRadius = flockSteering->getCohesionRadius();
	mAlignmentRadius = flockSteering->getAlignRadius();
	mSeparationRadius = flockSteering->getSeparationRadius();
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
		std::cout << "Error in data loading";
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
