#include <cassert>

#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

FaceSteering::FaceSteering(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, float targetRadius, float slowRadius, float timeToTarget)
{
	//mType = Steering::FACE;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
	mTargetRadius = targetRadius;
	mSlowRadius = slowRadius;
	mTimeToTarget = timeToTarget;
}

Steering * FaceSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	//vector to target.
	auto diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	//find rotation velocity
	float targetDirection = atan2(diff.getY(), diff.getX());
	float currentDirection = pOwner->getFacing();

	float rotation = getRotation(targetDirection, currentDirection);
	auto rotationSize = abs(rotation);
	float targetRotation;
	
	if (rotationSize < mTargetRadius) {
		data.rotAcc = 0;
		data.rotVel = 0;
		this->mData = data;
		return this;
	}
	if (rotationSize > mSlowRadius) {
		targetRotation = data.maxRotVel;
	}
	else
	{
		targetRotation = data.maxRotVel * rotationSize / mSlowRadius;
	}
	targetRotation *= rotation / rotationSize;
	data.rotAcc = targetRotation - currentDirection;
	data.rotAcc /= mTimeToTarget;
	auto angularAcceleration = abs(data.rotAcc);
	if (angularAcceleration > data.maxRotAcc) {
		data.rotAcc /= angularAcceleration;
		data.rotAcc *= data.maxRotAcc;
	}

	data.vel = 0; //holds unit in place, gets overwritten by steering using face.
	this->mData = data;
	return this;
}
