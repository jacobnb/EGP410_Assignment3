#include <Trackable.h>
#include "Steering.h"

class FaceSteering : public Steering
{
public:
	friend class SeekSteering;
	friend class ArriveAndFaceSteering;
	friend class FlockingSteering;
	friend class ArriveToAllSteering;
	friend class FlowSteering;
	friend class WanderPath;
	friend class PathfindFollow;
	friend class RunAwayPathfinding;

	FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc = Vector2D(0,0), const UnitID& targetID = INVALID_UNIT_ID, float targetRadius = 0.1, float slowRadius = 0.3, float timeToTarget = 0.1);
protected:
	virtual Steering* getSteering();
private:
	float mTargetRadius;
	float mSlowRadius;
	float mTimeToTarget;
};