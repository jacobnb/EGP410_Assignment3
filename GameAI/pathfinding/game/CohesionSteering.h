#pragma once
#include <Trackable.h>
#include "Steering.h"
#include <vector>

class Unit;
class SeekSteering;

class CohesionSteering : public Steering
{
public:
	friend class FlockingSteering;
	CohesionSteering(const UnitID& ownerID, float detectionRadius = 200);
	~CohesionSteering();
	inline float getRadius() { return mDetectRadius; };
	inline void setRadius(const float detectionRadius) { mDetectRadius = detectionRadius; };
	inline void incrementRadius(const float toAdd) { mDetectRadius += toAdd; };
protected:
	virtual Steering* getSteering();
private:
	float mDetectRadius;
	SeekSteering* mpSeekSteering;
	Vector2D getCenterOfUnits(std::vector<Vector2D>* unitLocations);
};