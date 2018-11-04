#pragma once
#include <Trackable.h>
#include "Steering.h"

class FaceSteering;
class SeekSteering : public Steering
{
public:
	friend class WanderSteering;
	friend class WanderAndChaseSteering;
	friend class CohesionSteering;
	friend class SeparationSteering;
	friend class AlignSteering;

	SeekSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	~SeekSteering();
protected:
	virtual Steering* getSteering();
	FaceSteering* mpFaceSteering;
};