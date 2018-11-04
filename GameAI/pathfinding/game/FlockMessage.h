#pragma once
#include "GameMessage.h"

enum FlockParameter {
	COHESION_FACTOR,
	SEPARATION_FACTOR,
	ALIGNMENT_FACTOR,
	WANDER_FACTOR,
	COHESION_RADIUS,
	ALIGNMENT_RADIUS,
	SEPARATION_RADIUS,
};

class FlockMessage : public GameMessage {
public:
	FlockMessage(FlockParameter param, float increment);
	FlockMessage(FlockParameter param, bool shouldIncrease);
	~FlockMessage();
	
	void process();
private:
	FlockParameter mParam;
	float mIncrement;
};