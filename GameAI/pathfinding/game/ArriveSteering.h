#include <Trackable.h>
#include "Steering.h"

class ArriveSteering : public Steering
{
public:
	friend class ArriveAndFaceSteering;
	friend class ArriveToAllSteering;
	friend class WanderPath;
	ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, const float targetRadius = 0, const float slowRadius = 100, const float timeToTarget = 0.1);
protected:
	virtual Steering* getSteering();
	bool finishedSteering;
private:
	float mTargetRadius;
	float mSlowRadius;
	float mTimeToTarget;
};