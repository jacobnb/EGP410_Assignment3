#include "CollisionMessage.h"

CollisionMessage::CollisionMessage(Unit * unit, Unit * other)
	:GameMessage(COLLISION_MESSAGE)
	,unit1(unit)
	,unit2(other)
{
}

CollisionMessage::~CollisionMessage()
{
}

void CollisionMessage::process()
{
	unit1->onCollision(unit2); //Unit1 called first so that it can access data on unit 2 which may delete itself.
	unit2->onCollision(unit1);
}
