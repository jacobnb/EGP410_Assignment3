#pragma once
#include <Trackable.h>
class DataLoader : public Trackable
{
public:
	void loadData();
	void writeData();
	inline float getCohesionFactor() { return mCohesionFactor; };
	inline float getSeparationFactor() { return mSeparationFactor; };
	inline float getAlignmentFactor() { return mAlignmentFactor; };
	inline float getWanderFactor() { return mWanderFactor; };
	inline float getCohesionRadius() { return mCohesionRadius; };
	inline float getAlignmentRadius() { return mAlignmentRadius; };
	inline float getSeparationRadius() { return mSeparationRadius; };
	inline float getFactorIncrease() { return mFactorIncrease; };
	inline float getRadiusIncrease() { return mRadiusIncrease; };
	void updateFlockingData(); //find a unit with flocking and copy its data
private:
	const std::string mFilePath = "data.txt";
	float stringToFloat(std::string str);

	float mCohesionFactor;
	float mSeparationFactor;
	float mAlignmentFactor;
	float mWanderFactor;
	float mCohesionRadius;
	float mAlignmentRadius;
	float mSeparationRadius;

	float mFactorIncrease = 0.1;
	float mRadiusIncrease = 50;
};