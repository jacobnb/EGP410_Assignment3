#include "Unit.h"
#include <assert.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "Component.h"
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "SteeringComponent.h"
#include "ComponentManager.h"
#include "SpriteManager.h"
#include "StateMachine.h"


Unit::Unit(const Sprite& sprite) 
	:mSprite(sprite)
	,mPositionComponentID(INVALID_COMPONENT_ID)
	,mPhysicsComponentID(INVALID_COMPONENT_ID)
	,mSteeringComponentID(INVALID_COMPONENT_ID)
	,mShowTarget(false)
{
	enabled = true;
}

Unit::~Unit()
{
	delete mStateMachine;
}

void Unit::draw() const
{
	if (enabled) {
		PositionComponent* pPosition = getPositionComponent();
		assert(pPosition != NULL);
		const Vector2D& pos = pPosition->getPosition();
		gpGame->getGraphicsSystem()->draw(mSprite, pos.getX(), pos.getY(), pPosition->getFacing());

		if (mShowTarget)
		{
			SteeringComponent* pSteering = getSteeringComponent();
			assert(pSteering != NULL);
			const Vector2D& targetLoc = pSteering->getTargetLoc();
			if (&targetLoc != &ZERO_VECTOR2D)
			{
				Sprite* pTargetSprite = gpGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID);
				assert(pTargetSprite != NULL);
				gpGame->getGraphicsSystem()->draw(*pTargetSprite, targetLoc.getX(), targetLoc.getY());
			}
		}
	}

}

float Unit::getFacing() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	return pPosition->getFacing();
}

void Unit::update(float elapsedTime)
{
	if(mStateMachine->getSizeOfMachine() > 0){
		mStateMachine->update();
	}
}

Unit::TYPE Unit::onCollision(Unit * other)
{ //this would probably be cleaner with inheritance.
	switch (other->getType()) {
		case NONE:
			break;
		case PLAYER:
			if (mType == COIN) {
				//despawn.
			}
			else if (mType == MIGHTY_CANDY) {
				//despawn, wait 60, respawn.
			}
			else if (mType == ENEMY) {
				//check if player is powered up.
			}
			break;
		case ENEMY:
			if (mType == PLAYER) {
				//Check if player is powered up.
			}
			else if (mType == ENEMY_FOOD) {
				//Despawn.
			}
			break;
		case ENEMY_FOOD:
			if (mType == ENEMY) {
				//eat food.
			}
			break;
		case MIGHTY_CANDY:
			if (mType == PLAYER) {
				//eat mighty candy
			}
			break;
		case COIN:
			if (mType == PLAYER) {
				//get points.
			}
			break;
		default:
			break;
	}
	return mType;
}

PositionComponent* Unit::getPositionComponent() const
{
	return mpPositionComponent;
}

PhysicsComponent* Unit::getPhysicsComponent() const
{
	PhysicsComponent* pComponent = gpGame->getComponentManager()->getPhysicsComponent(mPhysicsComponentID);
	return pComponent;
}

SteeringComponent* Unit::getSteeringComponent() const
{
	SteeringComponent* pComponent = gpGame->getComponentManager()->getSteeringComponent(mSteeringComponentID);
	return pComponent;
}

void Unit::setSteering(Steering::SteeringType type, Vector2D targetLoc /*= ZERO_VECTOR2D*/, UnitID targetUnitID /*= INVALID_UNIT_ID*/)
{
	SteeringComponent* pSteeringComponent = getSteeringComponent();
	if (pSteeringComponent != NULL)
	{
		pSteeringComponent->setData(SteeringData(type, targetLoc, mID, targetUnitID));
	}

}

void Unit::setSteering(Steering::SteeringType type, std::vector<Vector2D> targetLoc /*= ZERO_VECTOR2D*/, UnitID targetUnitID /*= INVALID_UNIT_ID*/){
	SteeringComponent* pSteeringComponent = getSteeringComponent();
	if (pSteeringComponent != NULL)
	{
		pSteeringComponent->setData(SteeringData(type, targetLoc, mID, targetUnitID));
	}
}