#include <cassert>

#include "WanderSteering.h"
#include "FaceSteering.h"
#include "SeekSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


WanderSteering::WanderSteering(const UnitID & ownerID, const float & radius, float wanderOffset, float wanderRate)
{
	//mType = Steering::WANDER; //not sure this is neccessary
	setOwnerID(ownerID);
	mWanderRadius = radius;
	mWanderOffset = wanderOffset;
	mWanderRate = wanderRate;
	Vector2D tempLoc = Vector2D(0);
	mpSeekSteering = new SeekSteering(ownerID, tempLoc);
}

WanderSteering::~WanderSteering()
{
	delete mpSeekSteering;
}

Steering * WanderSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	mTargetLoc = getTarget();
	mpSeekSteering->mTargetLoc = mTargetLoc;
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	data = mpSeekSteering->getSteering()->getData();

	this->mData = data;
	return this;
}
//Vector2D WanderSteering::getTarget()
//{
//	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
//	float wanderOrientation = atan2(pOwner->getPhysicsComponent()->getVelocity().getY(), pOwner->getPhysicsComponent()->getVelocity().getX());
//	wanderOrientation += genRandomBinomial()* mWanderRate;
//	auto charOrientation = pOwner->getPhysicsComponent()->getVelocity();
//	auto targetOrientation = wanderOrientation + atan2(pOwner->getPhysicsComponent()->getVelocity().getY(), pOwner->getPhysicsComponent()->getVelocity().getX());
//	auto target = pOwner->getPositionComponent()->getPosition()
//		+ charOrientation * mWanderOffset;
//	target += Vector2D(cos(targetOrientation), sin(targetOrientation)) * mWanderRadius;
//
//	return target;
//}
Vector2D WanderSteering::getTarget()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	float wanderOrientation = pOwner->getFacing();
	wanderOrientation +=  genRandomBinomial()* mWanderRate;
	auto charOrientation = Vector2D(cos(pOwner->getFacing()), sin(pOwner->getFacing()));
	auto targetOrientation = wanderOrientation + pOwner->getFacing();
	auto target = pOwner->getPositionComponent()->getPosition() 
		+ charOrientation * mWanderOffset;
	target += Vector2D(cos(targetOrientation),sin(targetOrientation)) * mWanderRadius;

	return target;
}
