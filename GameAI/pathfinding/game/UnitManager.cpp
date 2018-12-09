#include <map>
#include <stack>

#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"
#include "ComponentManager.h"
#include "GraphicsSystem.h"
#include "GridGraph.h"
#include "Grid.h"
#include "GameMessage.h"
#include "CollisionMessage.h"
#include "GameApp.h"
#include "GameMessageManager.h"

UnitID UnitManager::msNextUnitID = PLAYER_UNIT_ID + 1;

using namespace std;

UnitManager::UnitManager(Uint32 maxSize)
	:mPool(maxSize, sizeof(Unit))
{
}

Unit* UnitManager::createUnit(const Sprite& sprite, bool shouldWrap, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id)
{
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		pUnit = new (ptr)Unit(sprite);//placement new

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = msNextUnitID;
			msNextUnitID++;
		}

		//place in map
		mUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;

		//create some components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		ComponentID id = pComponentManager->allocatePositionComponent(posData,shouldWrap);
		pUnit->mPositionComponentID = id;
		pUnit->mpPositionComponent = pComponentManager->getPositionComponent(id);
		pUnit->mPhysicsComponentID = pComponentManager->allocatePhysicsComponent(pUnit->mPositionComponentID, physicsData);
		pUnit->mSteeringComponentID = pComponentManager->allocateSteeringComponent(pUnit->mPhysicsComponentID);

		//set max's
		pUnit->mMaxSpeed = MAX_SPEED;
		pUnit->mMaxAcc = MAX_ACC;
		pUnit->mMaxRotAcc = MAX_ROT_ACC;
		pUnit->mMaxRotVel = MAX_ROT_VEL;

	}

	return pUnit;
}


Unit* UnitManager::createPlayerUnit(const Sprite& sprite, bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/)
{
	return createUnit(sprite, shouldWrap, posData, physicsData, PLAYER_UNIT_ID);
}


Unit* UnitManager::createRandomUnit(const Sprite& sprite)
{
	int posX = rand() % gpGame->getGraphicsSystem()->getWidth();
	int posY = rand() % gpGame->getGraphicsSystem()->getHeight();
	int velX = rand() % 50 - 25;
	int velY = rand() % 40 - 20;
	Unit* pUnit = createUnit(sprite, true, PositionData(Vector2D(posX,posY),0)); //, PhysicsData(Vector2D(velX,velY),Vector2D(0.1f,0.1f), 0.1f, 0.05f)
	if (pUnit != NULL)
	{
		//pUnit->setSteering(Steering::SEEK, Vector2D(rand() % gpGame->getGraphicsSystem()->getWidth(), rand() % gpGame->getGraphicsSystem()->getHeight()));
		pUnit->setSteering(Steering::SEEK, Vector2D(gpGame->getGraphicsSystem()->getWidth()/2, gpGame->getGraphicsSystem()->getHeight()/2));
	}
	return pUnit;
}

Unit* UnitManager::CreateRandomUnitNoWall(const Sprite& sprite, GridGraph* graph){
	int posX = rand() % gpGame->getGraphicsSystem()->getWidth();
	int posY = rand() % gpGame->getGraphicsSystem()->getHeight();
	int velX = rand() % 50 - 25;
	int velY = rand() % 40 - 20;
	Unit* pUnit = createUnit(sprite, true, PositionData(Vector2D(posX,posY),0)); //, PhysicsData(Vector2D(velX,velY),Vector2D(0.1f,0.1f), 0.1f, 0.05f)
	Grid* grid = graph->getGrid();

	int index = grid->getSquareIndexFromPixelXY(posX, posY);
	while(grid->getValueAtIndex(index) == BLOCKING_VALUE){
		int posX = rand() % gpGame->getGraphicsSystem()->getWidth();
		int posY = rand() % gpGame->getGraphicsSystem()->getHeight();
		if(pUnit != NULL){
			deleteUnit(pUnit->GetID());
			pUnit = createUnit(sprite, true, PositionData(Vector2D(posX,posY),0));
			index = grid->getSquareIndexFromPixelXY(posX, posY);
		}
	}

	if (pUnit != NULL)
	{
		//pUnit->setSteering(Steering::SEEK, Vector2D(rand() % gpGame->getGraphicsSystem()->getWidth(), rand() % gpGame->getGraphicsSystem()->getHeight()));
		pUnit->setSteering(Steering::SEEK, Vector2D(gpGame->getGraphicsSystem()->getWidth()/2, gpGame->getGraphicsSystem()->getHeight()/2));
	}
	return pUnit;
}

Unit* UnitManager::getUnit(const UnitID& id) const
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::deleteUnit(const UnitID& id)
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

		//remove from map
		mUnitMap.erase(it);

		//remove components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		pComponentManager->deallocatePhysicsComponent(pUnit->mPhysicsComponentID);
		pComponentManager->deallocatePositionComponent(pUnit->mPositionComponentID);
		pComponentManager->deallocateSteeringComponent(pUnit->mSteeringComponentID);

		//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}

void UnitManager::deleteRandomUnit()
{
	if (mUnitMap.size() >= 1)
	{
		Uint32 target = rand() % mUnitMap.size();
		if (target == 0)//don't allow the 0th element to be deleted as it is the player unit
		{
			target = 1;
		}
		Uint32 cnt = 0;
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it, cnt++)
		{
			if (cnt == target)
			{
				deleteUnit(it->first);
				break;
			}
		}
	}
}

void UnitManager::drawAll() const
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->draw();
	}
}

void UnitManager::updateAll(float elapsedTime)
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->update(elapsedTime);
	}
}

void UnitManager::runCollisions()
{
	//add player to stack
	auto unit = getPlayerUnit();
	std::stack<Unit*> movingUnits;
	movingUnits.push(unit);
	GameMessage *pMessage = nullptr;
	//iterate through units, check for collisions, add enemies to stack.
	while (!movingUnits.empty()) {
		unit = movingUnits.top();
		movingUnits.pop();
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it) {
			auto other = it->second;
			if (other->getType() != unit->getType() && other->getType() != Unit::PLAYER) {
				//check if touching.
				Vector2D diff = unit->getPositionComponent()->getPosition() - other->getPositionComponent()->getPosition();
				bool colliding = diff.getLength() < (unit->getCollisionRadius() + other->getCollisionRadius());
				if (colliding) {
					pMessage = new CollisionMessage(unit, other);
					static_cast<GameApp*>(gpGame)->getMessageManager()->addMessage(pMessage, 0);
				}

				//this will only trigger during the first pass when unit type == player
				//add enemy units to stack
				if (other->getType() == Unit::ENEMY) {
					movingUnits.push(other);
				}
			}
		}

	}
	
}

std::vector<Unit*> UnitManager::getAllUnits()
{
	//https://stackoverflow.com/questions/110157/how-to-retrieve-all-keys-or-values-from-a-stdmap-and-put-them-into-a-vector

	vector<Unit*> units;
	units.reserve(mUnitMap.size());
	for (map<UnitID, Unit*>::iterator it = mUnitMap.begin(); it != mUnitMap.end(); ++it) {
		units.push_back(it->second);
	}
	units.shrink_to_fit();
	return units;
}
