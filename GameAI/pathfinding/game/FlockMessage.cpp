#include "FlockMessage.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Steering.h"
#include "SteeringComponent.h"
#include "FlockingSteering.h"
#include "DataLoader.h"
#include <vector>
FlockMessage::FlockMessage(FlockParameter param, float increment)
	:GameMessage(FLOCK_MESSAGE)
	, mParam(param)
	, mIncrement(increment)
{
}

FlockMessage::FlockMessage(FlockParameter param, bool shouldIncrease)
	:GameMessage(FLOCK_MESSAGE)
	,mParam(param)
{
	if (mParam < COHESION_RADIUS) { //if adjusting a factor
		mIncrement = gpGame->getDataLoader()->getFactorIncrease();
	}
	else { //adjusting a radius.
		mIncrement = gpGame->getDataLoader()->getRadiusIncrease();
	}
	if (!shouldIncrease) { //decreasing
		mIncrement *= -1;
	}
}

FlockMessage::~FlockMessage()
{
}

void FlockMessage::process()
{
	//==Init Array==//
	std::vector<FlockingSteering*> unitSteering;
	std::vector<Unit*> units = gpGame->getUnitManager()->getAllUnits();
	FlockingSteering* fs = nullptr;
	for (std::vector<Unit*>::iterator iter = units.begin();
		iter != units.end(); iter++) {
		fs = (*iter)->getSteeringComponent()->getFlockingSteering();
		if (fs != nullptr) {
			unitSteering.push_back(fs);
		}
	}


	//==Incremement values==//
	switch (mParam) {
	case COHESION_FACTOR: {
		for (std::vector<FlockingSteering*>::iterator iter = unitSteering.begin(); iter != unitSteering.end(); iter++) {
				(*iter)->incrementCohesionFactor(mIncrement);
		}
		break;
	}
	case SEPARATION_FACTOR: {
		for (std::vector<FlockingSteering*>::iterator iter = unitSteering.begin(); iter != unitSteering.end(); iter++) {
			(*iter)->incrementSeparationFactor(mIncrement);
		}
		break;
	}
	case ALIGNMENT_FACTOR: {
		for (std::vector<FlockingSteering*>::iterator iter = unitSteering.begin(); iter != unitSteering.end(); iter++) {
			(*iter)->incrementAlignmentFactor(mIncrement);
		}
		break;
	}
	case WANDER_FACTOR: {
		for (std::vector<FlockingSteering*>::iterator iter = unitSteering.begin(); iter != unitSteering.end(); iter++) {
			(*iter)->incrementWanderFactor(mIncrement);
		}
		break;
	}
	case COHESION_RADIUS: {
		for (std::vector<FlockingSteering*>::iterator iter = unitSteering.begin(); iter != unitSteering.end(); iter++) {
			(*iter)->incrementCohesionRadius(mIncrement);
		}
		break;
	}
	case ALIGNMENT_RADIUS: {
		for (std::vector<FlockingSteering*>::iterator iter = unitSteering.begin(); iter != unitSteering.end(); iter++) {
			(*iter)->incrementAlignRadius(mIncrement);
		}
		break;
	}
	case SEPARATION_RADIUS: {
		for (std::vector<FlockingSteering*>::iterator iter = unitSteering.begin(); iter != unitSteering.end(); iter++) {
			(*iter)->incrementSeparationRadius(mIncrement);
		}
		break;
	}
	default: {
		std::cout << "Error in Flock Message";
	}
	}
	gpGame->getDataLoader()->updateFlockingData();
	//this updates it for the UI display in game.
}
