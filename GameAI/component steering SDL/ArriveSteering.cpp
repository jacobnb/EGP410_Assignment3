#include <cassert>

#include "ArriveSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

ArriveSteering::ArriveSteering(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, float targetRadius, float slowRadius, float timeToTarget)
{
	//mType = Steering::ARRIVE;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
	mTargetRadius = targetRadius;
	mSlowRadius = slowRadius;
	mTimeToTarget = timeToTarget;
}

Steering * ArriveSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}
	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	auto distance = diff.getLength();
	//diff.normalize();
	//diff *= pOwner->getMaxAcc();
	float targetSpeed;
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	if (distance < mTargetRadius) {
		return this;
	}
	if (distance > mSlowRadius) {
		targetSpeed = data.maxSpeed;
	}
	else {
		targetSpeed = data.maxSpeed * distance / mSlowRadius;
	}
	auto targetVelocity = diff;
	targetVelocity.normalize();
	targetVelocity *= targetSpeed;

	
	data.acc = targetVelocity - data.vel;
	data.acc /= mTimeToTarget;

	if (data.acc.getLength() > data.maxAccMagnitude) {
		data.acc.normalize();
		data.acc *= data.maxAccMagnitude;
	}

	//===Face code===//
	//find rotation velocity
	/*float velocityDirection = atan2(diff.getY(), diff.getX());
	float currentDirection = pOwner->getFacing();
	float rotation = getRotation(velocityDirection, currentDirection);
	data.rotVel = rotation * 2;
*/
	this->mData = data;
	return this;
}
