#pragma once

#include "GameMessage.h"

#include "Vector2D.h"

//==Message to generate a path based on click location==//
class PathfindingMessage :public GameMessage
{
public:
	PathfindingMessage(const Vector2D& center, const int id);
	~PathfindingMessage();

	const Vector2D& getTo() const { return mCenter; };

	void process();

private:

	Vector2D mCenter;
	int mId;
};