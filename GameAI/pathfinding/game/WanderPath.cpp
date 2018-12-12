#include <cassert>

#include "WanderPath.h"
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
#include <time.h>

//Pathfinding but it moves to random positions. It is less a pathfinder and more a super pathfinder that is self contained, and only needs to be called to work.
WanderPath::WanderPath(const UnitID & ownerID, const UnitID& targetID = INVALID_UNIT_ID, const float targetRadius = 0, const float slowRadius = 100, const float timeToTarget = 0.1)
{
	mTargetRadius = targetRadius;
	mTimeToTarget = timeToTarget;
	mSlowRadius = slowRadius;
	index = 0;
	setOwnerID(ownerID);
	setTargetID(targetID);
	GenerateNewPath();
	if(mTargetVector.size() == 0){
		setTargetLoc(ZERO_VECTOR2D);
		mpFaceSteering = new FaceSteering(ownerID, ZERO_VECTOR2D, targetID);
		mpArriveSteering = new ArriveSteering(ownerID, ZERO_VECTOR2D, targetID, targetRadius, slowRadius, timeToTarget);
	}
	else {
		setTargetLoc(mTargetVector[0]);
		mpFaceSteering = new FaceSteering(ownerID, mTargetVector[0], targetID);
		mpArriveSteering = new ArriveSteering(ownerID, mTargetVector[0], targetID, targetRadius, slowRadius, timeToTarget);
	}
}

WanderPath::~WanderPath()
{
	delete mpFaceSteering;
	delete mpArriveSteering;
}

Steering * WanderPath::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	data = mpArriveSteering->getSteering()->getData();
	if(mpArriveSteering->finishedSteering){
		if(index + 1 >= mTargetVector.size()){
			GenerateNewPath();
			index = 0;
			mpArriveSteering->finishedSteering = false;
			pOwner->isFinished = true;
			delete mpFaceSteering;
			delete mpArriveSteering;
			if(mTargetVector.size() == 0){
				mTargetVector.push_back(pOwner->getPositionComponent()->getPosition());
			}
			mpFaceSteering = new FaceSteering(mOwnerID, mTargetVector[index], mTargetID);
			mpArriveSteering = new ArriveSteering(mOwnerID, mTargetVector[index], mTargetID, mTargetRadius, mSlowRadius, mTimeToTarget);
			this->mData = data;
			return this;
		}
		ArriveAtNewPoint();
		mpArriveSteering->finishedSteering = false;
	}
	auto faceData = mpFaceSteering->getSteering()->getData();
	data.rotAcc = faceData.rotAcc;
	data.rotVel = faceData.rotVel;
	if(mOwnerID != 0){
		data.vel = Vector2D(data.vel.getX() * .8f, data.vel.getY() * .8f);
	}

	this->mData = data;
	return this;
}

void WanderPath::ArriveAtNewPoint(){
	index++;
	delete mpFaceSteering;
	delete mpArriveSteering;
	mpFaceSteering = new FaceSteering(mOwnerID, mTargetVector[index], mTargetID);
	mpArriveSteering = new ArriveSteering(mOwnerID, mTargetVector[index], mTargetID, mTargetRadius, mSlowRadius, mTimeToTarget);
}

void WanderPath::GenerateNewPath(){
	int iterator = 0;
	mTargetVector.clear();
	bool stoppingNow = false;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PositionData data = pOwner->getPositionComponent()->getData();
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	GridPathfinder* pPathfinder = pGame->getPathfinder();
	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();
	int fromIndex = pGrid->getSquareIndexFromPixelXY(data.pos.getX(), data.pos.getY());

	Node* pFromNode = pGridGraph->getNode(fromIndex);
	Node* pToNode = pGridGraph->getRandomNode();
	Path* path = pGame->getPathfinder()->findPath(pToNode, pFromNode);		
	
	while(!path || path->getNumNodes() > 200){
		iterator++;
		srand(time(NULL) + rand() % 100);
		Node* pToNode = pGridGraph->getRandomNode();
		path = pGame->getPathfinder()->findPath(pFromNode, pToNode);
		if(iterator > 1){
			srand(time(NULL) + rand() % 100);
			stoppingNow = true;
			break;
		}
	}
	if(!stoppingNow && path){
		for(int c = 0; c < path->getNumNodes(); c++)
		{
			mTargetVector.push_back(pGrid->getULCornerOfSquare(path->peekNode(c)->getId()));
		}
	}
}