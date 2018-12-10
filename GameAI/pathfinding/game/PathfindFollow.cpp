#include <cassert>

#include "PathfindFollow.h"
#include "FaceSteering.h"
#include "ArriveSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "GraphicsSystem.h"
#include "GridPathfinder.h"
#include "GameApp.h"
#include "Grid.h"
#include "GridGraph.h"
#include "Path.h"

//This just follows a target using pathfinding
PathfindFollow::PathfindFollow(const UnitID & ownerID, const UnitID& targetID = INVALID_UNIT_ID, const float targetRadius = 0, const float slowRadius = 100, const float timeToTarget = 0.1)
{
	mTargetRadius = targetRadius;
	mTimeToTarget = timeToTarget;
	mSlowRadius = slowRadius;
	index = 0;
	setOwnerID(ownerID);
	setTargetID(targetID);
	mpFaceSteering = new FaceSteering(mOwnerID, ZERO_VECTOR2D, mTargetID);
	mpArriveSteering = new ArriveSteering(mOwnerID, ZERO_VECTOR2D, mTargetID, mTargetRadius, mSlowRadius, mTimeToTarget);
	AquireTarget();
}

PathfindFollow::~PathfindFollow()
{
	delete mpFaceSteering;
	delete mpArriveSteering;
}

Steering * PathfindFollow::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	data = mpArriveSteering->getSteering()->getData();
	if(mpArriveSteering->finishedSteering){
		if(index + 1 >= mTargetVector.size()){
			AquireTarget();
			mpArriveSteering->finishedSteering = false;
			index = 0;
			this->mData = data;
			return this;
		}
		ArriveAtNewPoint();
		mpArriveSteering->finishedSteering = false;
	}
	auto faceData = mpFaceSteering->getSteering()->getData();
	data.rotAcc = faceData.rotAcc;
	data.rotVel = faceData.rotVel;
	data.vel = Vector2D(data.vel.getX() * .8f, data.vel.getY() * .8f);

	this->mData = data;
	return this;
}

void PathfindFollow::ArriveAtNewPoint(){
	index++;
	delete mpFaceSteering;
	delete mpArriveSteering;
	mpFaceSteering = new FaceSteering(mOwnerID, mTargetVector[index], mTargetID);
	mpArriveSteering = new ArriveSteering(mOwnerID, mTargetVector[index], mTargetID, mTargetRadius, mSlowRadius, mTimeToTarget);
}

void PathfindFollow::AquireTarget(){
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
	PositionData data = pOwner->getPositionComponent()->getData();
	PositionData targetData = pTarget->getPositionComponent()->getData();
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	GridPathfinder* pPathfinder = pGame->getPathfinder();
	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();

	int fromIndex = pGrid->getSquareIndexFromPixelXY(data.pos.getX(), data.pos.getY());
	int toIndex = pGrid->getSquareIndexFromPixelXY(targetData.pos.getX(), targetData.pos.getY());

	Node* pFromNode = pGridGraph->getNode(fromIndex);
	Node* pToNode = pGridGraph->getNode(toIndex);
	Path* path = pGame->getPathfinder()->findPath(pToNode, pFromNode);		

	if(path){
		for(int c = 0; c < path->getNumNodes(); c++){
			mTargetVector.push_back(pGrid->getULCornerOfSquare(path->peekNode(c)->getId()));
		}
		delete mpFaceSteering;
		delete mpArriveSteering;
		mpFaceSteering = new FaceSteering(mOwnerID, mTargetVector[index], mTargetID);
		mpArriveSteering = new ArriveSteering(mOwnerID, mTargetVector[index], mTargetID, mTargetRadius, mSlowRadius, mTimeToTarget);
	}
}
