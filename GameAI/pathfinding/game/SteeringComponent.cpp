#include "ComponentManager.h"
#include "SteeringComponent.h"
#include "SeekSteering.h"
#include "WanderSteering.h"
#include "FaceSteering.h"
#include "ArriveSteering.h"
#include "ArriveAndFaceSteering.h"
#include "WanderAndChaseSteering.h"
#include "ArriveToAllSteering.h"
#include "flowSteering.h"
#include "WanderPath.h"
#include "PathfindFollow.h"
#include "RunAwayPathfinding.h"

SteeringComponent::SteeringComponent(const ComponentID& id, const ComponentID& physicsComponentID) 
	:Component(id)
	, mPhysicsComponentID(physicsComponentID)
	, mpSteering(NULL)
{
}

SteeringComponent::~SteeringComponent()
{
	delete mpSteering;
}

void SteeringComponent::applySteering(PhysicsComponent& physicsComponent)
{
	if (mpSteering != NULL)
	{
		//allow Steering to run
		mpSteering->update();
		//set physics data to that of the Steering
		physicsComponent.setData(mpSteering->getData());
		//update component's data
		mData.targetLoc = mpSteering->getTargetLoc();
	}
}


void SteeringComponent::setData(const SteeringData& data)
{
	mData = data;

	switch (data.type)
	{
		case Steering::SEEK:
		{
			//cleanup old steering - todo: check for already existing steering and reuse if possible
			delete mpSteering;
			//create new steering
			mpSteering = new SeekSteering(data.ownerID, data.targetLoc, data.targetID, false);
			break;
		}
		case Steering::FLEE:
		{
			//cleanup old steering - todo: check for already existing steering and reuse if possible
			delete mpSteering;
			mpSteering = new SeekSteering(data.ownerID, data.targetLoc, data.targetID, true);
			break;
		}
		case Steering::ARRIVE_FACE: 
		{
			delete mpSteering;
			mpSteering = new ArriveAndFaceSteering(data.ownerID, data.targetLoc, data.targetID);
			break;
		}
		case Steering::ARRIVE:
		{//cleanup old steering - todo: check for already existing steering and reuse if possible
			delete mpSteering;
			mpSteering = new ArriveSteering(data.ownerID, data.targetLoc, data.targetID);
			break;
		}
		case Steering::WANDER:
		{
			//cleanup old steering - todo: check for already existing steering and reuse if possible
			delete mpSteering;
			mpSteering = new WanderSteering(data.ownerID);
			break;
		}
		case Steering::WANDER_CHASE:
		{
			delete mpSteering;
			mpSteering = new WanderAndChaseSteering(data.ownerID, data.targetID);
			break;
		}
		case Steering::FACE:
		{	//cleanup old steering - todo: check for already existing steering and reuse if possible
			delete mpSteering;
			//create new steering
			mpSteering = new FaceSteering(data.ownerID, data.targetLoc, data.targetID);
			break;
		}
		case Steering::ARRIVETOALLSTEERING:
		{
			delete mpSteering;
			mpSteering = new ArriveToAllSteering(data.ownerID, data.vectorLoc, data.targetID, 20, 55, .1);
			break;
		}
		case Steering::FLOW:
		{
			delete mpSteering;
			mpSteering = new FlowSteering(data.ownerID);
			break;
		}
		case Steering::WANDERPATH:
		{
			delete mpSteering;
			mpSteering = new WanderPath(data.ownerID, data.targetID, 20, 55, .1);
			break;
		}
		case Steering::PATHFINDFOLLOW:
		{
			delete mpSteering;
			mpSteering = new PathfindFollow(data.ownerID, data.targetID, 20, 55, .1);
			break;
		}
		case Steering::RUNAWAYPATHFINDING:
		{
			delete mpSteering;
			mpSteering = new RunAwayPathfinding(data.ownerID, data.targetID, 20, 55, .1);
		}
		default:
		{

		}
	};
}

