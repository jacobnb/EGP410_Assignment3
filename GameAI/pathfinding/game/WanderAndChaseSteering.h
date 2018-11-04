#pragma once
#include <Trackable.h>
#include "Steering.h"

class SeekSteering;
class WanderSteering;
class WanderAndChaseSteering : public Steering
{
public:
	//need to pass radius and center?
	WanderAndChaseSteering(const UnitID& ownerID, const UnitID& targetID, float chaseDistance = 200);
	~WanderAndChaseSteering();
protected:
	virtual Steering* getSteering();
private:
	SeekSteering* mpSeekSteering;
	WanderSteering* mpWanderSteering;
	float mChaseDistance;
};