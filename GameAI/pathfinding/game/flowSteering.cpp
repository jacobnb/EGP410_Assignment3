#include <cassert>

#include "Steering.h"
#include "FaceSteering.h"
#include "flowSteering.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Grid.h"
#include "GridGraph.h"

FlowSteering::FlowSteering(const UnitID& ownerID)
	: Steering()
{
	mpFaceSteering = new FaceSteering(ownerID);

	setOwnerID(ownerID);
}

FlowSteering::~FlowSteering()
{
	delete mpFaceSteering;
}

Steering*  FlowSteering::getSteering()
{
	//get game data
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	//find node
	Vector2D pos = pOwner->getPositionComponent()->getPosition();
	int index = pGrid->getSquareIndexFromPixelXY((int)pos.getX(), (int)pos.getY());
	Vector2D dir = pGridGraph->getNode(index)->getDir();

	dir.normalize();
	dir *= pOwner->getMaxAcc();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	data.acc = dir;

	mTargetLoc = pos + dir;

	//Face target. This will probably still face while running away.
	mpFaceSteering->mTargetLoc = mTargetLoc;
	data.rotAcc = mpFaceSteering->getSteering()->getData().rotAcc;
	data.rotVel = mpFaceSteering->getSteering()->getData().rotVel;

	this->mData = data;
	return this;
}
