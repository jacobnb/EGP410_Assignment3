#include "SeparationSteering.h"
#include "SeekSteering.h"
#include "Game.h"
#include "Unit.h"
#include "UnitManager.h"

SeparationSteering::SeparationSteering(const UnitID & ownerID, float detectionRadius)
{
	mDetectRadius = detectionRadius;
	setOwnerID(ownerID);
	mpSeekSteering = new SeekSteering(ownerID, ZERO_VECTOR2D);
}

SeparationSteering::~SeparationSteering()
{
	delete mpSeekSteering;
}

Steering * SeparationSteering::getSteering()
{
	//set up
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	auto data = pOwner->getPhysicsComponent()->getData();

	//Find location
	Vector2D targetLoc = getVectorAwayFromUnits(
		getUnitLocsInRadius(
			pOwner->getPositionComponent()->getPosition(),
			mDetectRadius,
			gpGame->getUnitManager()->getAllUnits() //cache this in steering.h to speed up performcance
		),
		pOwner
	);
	targetLoc += pOwner->getPositionComponent()->getPosition();
	setTargetLoc(targetLoc);

	//Seek location
	mpSeekSteering->setTargetLoc(targetLoc);
	data = mpSeekSteering->getSteering()->getData();

	this->mData = data;
	return this;
}

Vector2D SeparationSteering::getVectorAwayFromUnits(std::vector<Vector2D>* unitLocations, const Unit* owner)
{
	Vector2D away = ZERO_VECTOR2D;
	auto selfLoc = owner->getPositionComponent()->getPosition();
	auto maxAcc = owner->getMaxAcc();
	for (std::vector<Vector2D>::iterator it = unitLocations->begin();
		it != unitLocations->end(); it++) {
		Vector2D diff = *it - selfLoc;
		float length = diff.getLength();
		if (length != 0) { //ignores self
			diff.normalize();
			away += diff*(maxAcc* mDetectRadius / length);
		}
		//could do something more complex, but this should work to weight it by distance
	}
	return away*-1;
}
