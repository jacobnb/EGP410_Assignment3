#pragma once
#include <Trackable.h>
#include "Steering.h"
#include <vector>

class Unit;
class SeekSteering;

class AlignSteering : public Steering
{
public:
	friend class FlockingSteering;
	AlignSteering(const UnitID& ownerID, float detectionRadius = 200);
	~AlignSteering();
	inline float getRadius() { return mDetectRadius; };
	inline void setRadius(const float detectionRadius) { mDetectRadius = detectionRadius; };
	inline void incrementRadius(const float toAdd) { mDetectRadius += toAdd;};
protected:
	virtual Steering* getSteering();
private:
	float mDetectRadius;
	SeekSteering* mpSeekSteering;
	Vector2D getAlignedVelocity(std::vector<Unit*> units);
	std::vector<Unit*> getUnitsInRadius(Vector2D ownerPosit, std::vector<Unit*> allUnits);
};