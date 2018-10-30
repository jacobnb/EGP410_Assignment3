#pragma once
#include <Trackable.h>
#include "Steering.h"
#include <vector>

class Unit;
class SeekSteering;

class SeparationSteering : public Steering
{
public:
	friend class FlockingSteering;
	SeparationSteering(const UnitID& ownerID, float detectionRadius = 100);
	~SeparationSteering();
	inline float getRadius() { return mDetectRadius; };
	inline void setRadius(const float detectionRadius) { mDetectRadius = detectionRadius; };
	inline void incrementRadius(const float toAdd) { mDetectRadius += toAdd; };
protected:
	virtual Steering* getSteering();
private:
	float mDetectRadius;
	SeekSteering* mpSeekSteering;
	Vector2D getVectorAwayFromUnits(std::vector<Vector2D>* unitLocations, const Unit* owner);
};