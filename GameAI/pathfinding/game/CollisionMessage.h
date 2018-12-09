#pragma once

#include "GameMessage.h"
#include "Unit.h"

//==Message fired on keypress, interprets keyboard input==//
class CollisionMessage :public GameMessage
{
public:
	CollisionMessage(Unit* unit, Unit* other);
	~CollisionMessage();

	void process();

private:
	Unit *unit1, *unit2;
};