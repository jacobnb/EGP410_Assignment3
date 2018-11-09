#pragma once
#include <Trackable.h>
#include "Steering.h"

class FaceSteering;
class ArriveSteering;

class ArriveToAllSteering : public Steering
{
public:
	ArriveToAllSteering(const UnitID & ownerID, std::vector<Vector2D> targetVector, const UnitID & targetID, const float targetRadius, const float slowRadius, const float timeToTarget);
	~ArriveToAllSteering();
protected:
	virtual Steering* getSteering();
private:
	std::vector<Vector2D> mTargetVector;
	int index;
	FaceSteering* mpFaceSteering;
	ArriveSteering* mpArriveSteering;
	float mTargetRadius;
	float mSlowRadius;
	float mTimeToTarget;

	void ArriveAtNewPoint();
};
