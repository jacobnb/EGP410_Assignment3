#include <cassert>

#include "ArriveAndFaceSteering.h"
#include "FaceSteering.h"
#include "ArriveSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

ArriveAndFaceSteering::ArriveAndFaceSteering(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, const float targetRadius, const float slowRadius, const float timeToTarget)
{
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
	mpFaceSteering = new FaceSteering(ownerID, targetLoc, targetID);
	mpArriveSteering = new ArriveSteering(ownerID, targetLoc, targetID, targetRadius, slowRadius, timeToTarget);
}

ArriveAndFaceSteering::~ArriveAndFaceSteering()
{
	delete mpFaceSteering;
	delete mpArriveSteering;
}

Steering * ArriveAndFaceSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	
	data = mpArriveSteering->getSteering()->getData(); 
	auto faceData = mpFaceSteering->getSteering()->getData();
	data.rotAcc = faceData.rotAcc;
	data.rotVel = faceData.rotVel;

	this->mData = data;
	return this;
}
