#include "CohesionSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "SeekSteering.h"
CohesionSteering::CohesionSteering(const UnitID & ownerID, float detectionRadius)
{
	mDetectRadius = detectionRadius;
	setOwnerID(ownerID);
	mpSeekSteering = new SeekSteering(ownerID, ZERO_VECTOR2D);
}

CohesionSteering::~CohesionSteering()
{
	delete mpSeekSteering;
}

Steering * CohesionSteering::getSteering()
{
	//set up
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	auto data = pOwner->getPhysicsComponent()->getData();

	//Find location
	Vector2D targetLoc = getCenterOfUnits(
		getUnitLocsInRadius(
			pOwner->getPositionComponent()->getPosition(),
			mDetectRadius,
			gpGame->getUnitManager()->getAllUnits() //cache this in steering.h to speed up performcance
		)
	);
	//We don't need to add player position b/c it's not taken out of the average

	setTargetLoc(targetLoc);

	//Seek location
	mpSeekSteering->setTargetLoc(targetLoc);
	data = mpSeekSteering->getSteering()->getData();
	this->mData = data;
	return this;
}

	
Vector2D CohesionSteering::getCenterOfUnits(std::vector<Vector2D>* unitLocations)
{
	Vector2D centerLoc = ZERO_VECTOR2D;
	for (std::vector<Vector2D>::iterator it = unitLocations->begin();
		it != unitLocations->end(); it++) {
		centerLoc += *it;
	}
	if (unitLocations->size() > 0) {
		centerLoc *= (1.0 / unitLocations->size());
	}
	return centerLoc;
}
