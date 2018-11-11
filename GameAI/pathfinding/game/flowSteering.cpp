#include <cassert>

#include "Steering.h"
#include "FaceSteering.h"
#include "flowSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


FlowSteering::FlowSteering(const UnitID& ownerID)
	: Steering()
{
	mpFaceSteering = new FaceSteering(ownerID);

	setOwnerID(ownerID);
}

FlowSteering::~FlowSteering()
{
	delete mpFaceSteering;
}




Steering*  FlowSteering::getSteering()
{
	Vector2D dir;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	dir.normalize();
	dir *= pOwner->getMaxAcc();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	data.acc = dir;

	//Face target. This will probably still face while running away.
	mpFaceSteering->mTargetLoc = mTargetLoc;
	data.rotAcc = mpFaceSteering->getSteering()->getData().rotAcc;
	data.rotVel = mpFaceSteering->getSteering()->getData().rotVel;

	this->mData = data;
	return this;
}
