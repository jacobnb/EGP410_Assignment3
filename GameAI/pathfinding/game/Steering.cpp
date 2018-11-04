#include "Steering.h"
#include "Unit.h"

void Steering::update()
{
	Steering* pTemp = getSteering();
	if (pTemp != NULL)
	{
		mData = pTemp->getData();
	}
}

float Steering::getRotation(float targetDirection, float currentDirection)
{
	float rotation = targetDirection - currentDirection;
	const float pi = (atan(1) * 4);
	//might have a problem if rotation is more than 2Pi
	if (rotation > pi) {
		rotation -= 2 * pi;
	}
	else if (rotation < -pi) {
		rotation += 2 * pi;
	}
	return rotation;
}

std::vector<Vector2D>* Steering::getUnitLocsInRadius(Vector2D ownerPosit, float radius, std::vector<Unit*> allUnits)
{
	//this gets all unit locations (including self) in mDetectRadius
	using std::vector;
	vector<Vector2D>* unitPosits = new vector<Vector2D>();

	for (vector<Unit*>::iterator it = allUnits.begin();
		it != allUnits.end(); it++) {
		Vector2D loc = (*it)->getPositionComponent()->getPosition();
		if ((ownerPosit - loc).getLength() < radius) {
			unitPosits->push_back(loc);
		}
	}
	return unitPosits;
}
