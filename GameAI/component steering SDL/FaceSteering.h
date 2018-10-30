#include <Trackable.h>
#include "Steering.h"

class FaceSteering : public Steering
{
public:
	friend class SeekSteering;
	friend class ArriveAndFaceSteering;
	friend class FlockingSteering;
	FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, float targetRadius = 0.1, float slowRadius = 0.3, float timeToTarget = 0.1);
protected:
	virtual Steering* getSteering();
private:
	float mTargetRadius;
	float mSlowRadius;
	float mTimeToTarget;
};