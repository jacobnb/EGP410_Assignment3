#include <Trackable.h>
#include "Steering.h"

class ArriveSteering : public Steering
{
public:
	friend class ArriveAndFaceSteering;
	ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, const float targetRadius = 0, const float slowRadius = 100, const float timeToTarget = 0.1);
protected:
	virtual Steering* getSteering();
private:
	float mTargetRadius;
	float mSlowRadius;
	float mTimeToTarget;
};