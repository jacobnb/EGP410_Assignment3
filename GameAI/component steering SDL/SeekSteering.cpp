#include <cassert>

#include "Steering.h"
#include "FaceSteering.h"
#include "SeekSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


SeekSteering::SeekSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	mpFaceSteering = new FaceSteering(ownerID, targetLoc, targetID);
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::SEEK;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

SeekSteering::~SeekSteering()
{
	delete mpFaceSteering;
}




Steering* SeekSteering::getSteering()
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

	if (mType == Steering::SEEK)
	{
		diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	}
	else
	{
		diff = pOwner->getPositionComponent()->getPosition() - mTargetLoc;
	}

	diff.normalize();
	diff *= pOwner->getMaxAcc();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	data.acc = diff;
	
	//Face target. This will probably still face while running away.
	mpFaceSteering->mTargetLoc = mTargetLoc;
	data.rotAcc = mpFaceSteering->getSteering()->getData().rotAcc;
	data.rotVel = mpFaceSteering->getSteering()->getData().rotVel;

	this->mData = data;
	return this;
}
