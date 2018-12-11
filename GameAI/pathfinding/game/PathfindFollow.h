#pragma once
#include <Trackable.h>
#include "Steering.h"

class FaceSteering;
class ArriveSteering;

class PathfindFollow : public Steering
{
public:
	PathfindFollow(const UnitID & ownerID, const UnitID & targetID, const float targetRadius, const float slowRadius, const float timeToTarget);
	~PathfindFollow();
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
	bool regeneratePath;

	void ArriveAtNewPoint();
	void AquireTarget();
};
