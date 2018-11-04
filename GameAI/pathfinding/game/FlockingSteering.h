#pragma once
#include <Trackable.h>
#include "Steering.h"

class AlignSteering;
class CohesionSteering;
class SeparationSteering;
class WanderSteering;
class FaceSteering;

class FlockingSteering : public Steering
{
public:
	FlockingSteering(const UnitID& ownerID);
	~FlockingSteering();

	//==getters and setters==//
	inline float getCohesionFactor() { return mCohesionMult; };
	inline float getSeparationFactor() { return mSeparationMult; };
	inline float getAlignmentFactor() { return mAlignMult; };
	inline float getWanderFactor() { return mWanderMult; };
	inline void incrementCohesionFactor(float toAdd) { mCohesionMult += toAdd; };
	inline void incrementSeparationFactor(float toAdd) { mSeparationMult += toAdd; };
	inline void incrementAlignmentFactor(float toAdd) { mAlignMult += toAdd; };
	inline void incrementWanderFactor(float toAdd) { mWanderMult += toAdd; };
	//I didn't make this inline so that the individual steering headers don't need to be included.
	void incrementCohesionRadius(float toAdd);
	void incrementAlignRadius(float toAdd);
	void incrementSeparationRadius(float toAdd);
	float getCohesionRadius();
	float getAlignRadius();
	float getSeparationRadius();


protected:
	virtual Steering* getSteering();
private:
	AlignSteering* mpAlignSteer;
	CohesionSteering* mpCohesionSteer;
	SeparationSteering* mpSeparationSteer;
	WanderSteering* mpWanderSteer;
	FaceSteering* mpFaceSteer;

	float mCohesionMult = 0.2;
	float mSeparationMult = 0.2;
	float mAlignMult = 0.3;
	float mWanderMult = 0.3;
};