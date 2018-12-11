#include <cassert>

#include "RunAwayPathfinding.h"
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

//Pathfinding but it moves away from the target in a random position
RunAwayPathfinding::RunAwayPathfinding(const UnitID & ownerID, const UnitID& targetID = INVALID_UNIT_ID, const float targetRadius = 0, const float slowRadius = 100, const float timeToTarget = 0.1)
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

RunAwayPathfinding::~RunAwayPathfinding()
{
	delete mpFaceSteering;
	delete mpArriveSteering;
}

Steering * RunAwayPathfinding::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	data = mpArriveSteering->getSteering()->getData();
	if(mpArriveSteering->finishedSteering){
		if(index + 1 >= mTargetVector.size()){
			GenerateNewPath();
			index = 0;
			mpArriveSteering->finishedSteering = false;
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

void RunAwayPathfinding::ArriveAtNewPoint(){
	index++;
	delete mpFaceSteering;
	delete mpArriveSteering;
	mpFaceSteering = new FaceSteering(mOwnerID, mTargetVector[index], mTargetID);
	mpArriveSteering = new ArriveSteering(mOwnerID, mTargetVector[index], -1, mTargetRadius, mSlowRadius, mTimeToTarget);
}

void RunAwayPathfinding::GenerateNewPath(){
	int iterator = 0;
	mTargetVector.clear();
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
	PositionData data = pOwner->getPositionComponent()->getData();
	PositionData targetData = pTarget->getPositionComponent()->getData();
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	GridPathfinder* pPathfinder = pGame->getPathfinder();
	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();
	int fromIndex = pGrid->getSquareIndexFromPixelXY(data.pos.getX(), data.pos.getY());
	int toIndexX;
	int toIndexY;
	int gameWidth = gpGame->getGraphicsSystem()->getWidth() / 2;
	int gameHeight = gpGame->getGraphicsSystem()->getHeight() / 2;
	if(targetData.pos.getX() > gameWidth){
		//The player is on the top half of x, move to bottom half
		toIndexX = rand() % gameWidth / 2;
	}
	else {
		//The player is on the bottom half of x, move to top half
		toIndexX = rand() % gameWidth / 2 +  gameWidth;
	}

	if(targetData.pos.getY() > gameHeight){
		//The player is on the top half of x, move to bottom half
		toIndexY = rand() % gameHeight / 2;
	}
	else {
		//The player is on the bottom half of x, move to top half
		toIndexY = rand() % gameHeight / 2 +  gameHeight;
	}

	int toIndex =  pGrid->getSquareIndexFromPixelXY(toIndexX, toIndexY);
	Node* pFromNode = pGridGraph->getNode(fromIndex);
	Node* pToNode = pGridGraph->getNode(toIndex);
	Path* path = pGame->getPathfinder()->findPath(pToNode, pFromNode);		

	while(!path || path->getNumNodes() > 200){
		iterator++;
		if(targetData.pos.getX() > gameWidth){
			//The player is on the top half of x, move to bottom half
			toIndexX = rand() % gameWidth / 2;
		}
		else {
			//The player is on the bottom half of x, move to top half
			toIndexX = rand() % gameWidth / 2 +  gameWidth;
		}

		if(targetData.pos.getY() > gameHeight){
			//The player is on the top half of x, move to bottom half
			toIndexY = rand() % gameHeight / 2;
		}
		else {
			//The player is on the bottom half of x, move to top half
			toIndexY = rand() % gameHeight / 2 +  gameHeight;
		}

		int toIndex =  pGrid->getSquareIndexFromPixelXY(toIndexX, toIndexY);
		Node* pToNode = pGridGraph->getNode(toIndex);
		Path* path = pGame->getPathfinder()->findPath(pToNode, pFromNode);

		if(iterator > 10){
			srand(time(NULL));
			return;
		}
	}
	for(int c = 0; c < path->getNumNodes(); c++)
	{
		mTargetVector.push_back(pGrid->getULCornerOfSquare(path->peekNode(c)->getId()));
	}
}