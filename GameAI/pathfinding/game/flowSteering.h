#pragma once
#include <Trackable.h>
#include "Steering.h"

class FaceSteering;
class FlowSteering : public Steering
{
public:
	FlowSteering(const UnitID& ownerID);
	~FlowSteering();
protected:
	virtual Steering* getSteering();
	FaceSteering* mpFaceSteering;
};