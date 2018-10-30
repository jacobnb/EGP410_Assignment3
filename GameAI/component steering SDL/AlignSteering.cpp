#include "AlignSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "SeekSteering.h"

AlignSteering::AlignSteering(const UnitID & ownerID, float detectionRadius)
{
	mDetectRadius = detectionRadius;
	setOwnerID(ownerID);
	mpSeekSteering = new SeekSteering(ownerID, ZERO_VECTOR2D);
}

AlignSteering::~AlignSteering()
{
	delete mpSeekSteering;
}

Steering * AlignSteering::getSteering()
{
	//set up
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	auto data = pOwner->getPhysicsComponent()->getData();

	//Find location
	Vector2D targetLoc = getAlignedVelocity(
		getUnitsInRadius(
			pOwner->getPositionComponent()->getPosition(),
			gpGame->getUnitManager()->getAllUnits() //cache this in steering.h to speed up performcance
		)
	);
	targetLoc += pOwner->getPositionComponent()->getPosition();
	setTargetLoc(targetLoc);

	//Seek location
	mpSeekSteering->setTargetLoc(targetLoc);
	data = mpSeekSteering->getSteering()->getData();

	this->mData = data;
	return this;
}

Vector2D AlignSteering::getAlignedVelocity(std::vector<Unit*> units)
{
	Vector2D alignedVel = ZERO_VECTOR2D;

	//get combined velocities.
	for (std::vector<Unit*>::iterator it = units.begin();
		it != units.end(); it++) {
		alignedVel += (*it)->getPhysicsComponent()->getVelocity();
	}
	return alignedVel;
}

std::vector<Unit*> AlignSteering::getUnitsInRadius(Vector2D ownerPosit, std::vector<Unit*> allUnits)
{	//this gets all units (including self) in mDetectRadius
	using std::vector;
	vector<Unit*> units = vector<Unit*>();

	for (vector<Unit*>::iterator it = allUnits.begin();
		it != allUnits.end(); it++) {
		Vector2D loc = (*it)->getPositionComponent()->getPosition();
		if ((ownerPosit - loc).getLength() < mDetectRadius) {
			units.push_back(*it);
		}
	}
	units.shrink_to_fit();
	return units;
}

