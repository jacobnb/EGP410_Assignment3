#include <Trackable.h>
#include "Steering.h"

class FaceSteering;
class ArriveSteering;
class ArriveAndFaceSteering : public Steering
{
public:
	ArriveAndFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, const float targetRadius = 0, const float slowRadius = 100, const float timeToTarget = 0.1);
	~ArriveAndFaceSteering();
protected:
	virtual Steering* getSteering();
private:
	FaceSteering* mpFaceSteering;
	ArriveSteering* mpArriveSteering;
};