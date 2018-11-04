#include <cassert>

#include "WanderSteering.h"
#include "SeekSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "WanderAndChaseSteering.h"



WanderAndChaseSteering::WanderAndChaseSteering(const UnitID & ownerID, const UnitID & targetID, float chaseDistance)
{
	//mType = Steering::WANDER; //not sure this is neccessary
	setOwnerID(ownerID);
	setTargetID(targetID);
	Vector2D tempLoc = Vector2D(0);
	mpSeekSteering = new SeekSteering(ownerID, tempLoc, targetID);
	mpWanderSteering = new WanderSteering(ownerID);
	mChaseDistance = chaseDistance;
}


WanderAndChaseSteering::~WanderAndChaseSteering()
{
	delete mpSeekSteering;
	delete mpWanderSteering;
}

Steering * WanderAndChaseSteering::getSteering()
{
	//set up
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
	auto data = pOwner->getPhysicsComponent()->getData();
	mTargetLoc = pTarget->getPositionComponent()->getPosition();
	auto diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	//chase or wander
	if (diff.getLength() > mChaseDistance) {
		data = mpWanderSteering->getSteering()->getData();
	}
	else {
		data = mpSeekSteering->getSteering()->getData();
	}

	this->mData = data;
	return this;
}
