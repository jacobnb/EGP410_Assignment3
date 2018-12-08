#pragma once
#include <Trackable.h>
#include "Steering.h"

class FaceSteering;
class ArriveSteering;
class Path;

class WanderPath : public Steering
{
public:
	WanderPath(const UnitID & ownerID, Vector2D patrol1, Vector2D patrol2, const UnitID & targetID, const float targetRadius, const float slowRadius, const float timeToTarget);
	~WanderPath();
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
	void GenerateNewPath();
};
