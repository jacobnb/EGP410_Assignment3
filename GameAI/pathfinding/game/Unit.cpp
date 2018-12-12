#include "Unit.h"
#include <assert.h>

#include "Game.h"
#include "GameApp.h"
#include "GraphicsSystem.h"
#include "PhysicsComponent.h"
#include "SteeringComponent.h"
#include "ComponentManager.h"
#include "SpriteManager.h"
#include "StateMachine.h"
#include "UnitManager.h"
#include "DataLoader.h"
#include "GraphicsBufferManager.h"

Unit::Unit(const Sprite& sprite) 
	:mSprite(sprite)
	,mPositionComponentID(INVALID_COMPONENT_ID)
	,mPhysicsComponentID(INVALID_COMPONENT_ID)
	,mSteeringComponentID(INVALID_COMPONENT_ID)
	,mShowTarget(false)
{
	enabled = true;
	mpAltSprite = nullptr;
	mHealth = gpGame->getDataLoader()->getData(DataLoader::PLAYER_HEALTH);
}

Unit::~Unit(){
	delete mStateMachine;
}

void Unit::despawn(float spawnTime)
{
	enabled = false;
	disabledTimer = spawnTime;
}

void Unit::update_checkRespawn(float elapsedTime)
{
	disabledTimer -= elapsedTime;
	if (disabledTimer < 0) {
		setActive(true);
	}
}

void Unit::update_checkPower(float elapsedTime)
{
	poweredTimer -= elapsedTime;
}

void Unit::draw() const
{
	if (enabled) {
		PositionComponent* pPosition = getPositionComponent();
		assert(pPosition != NULL);
		const Vector2D& pos = pPosition->getPosition();
		if (poweredUp() && mpAltSprite) {
			gpGame->getGraphicsSystem()->draw(*mpAltSprite, pos.getX(), pos.getY(), pPosition->getFacing());
		}
		else {
			gpGame->getGraphicsSystem()->draw(mSprite, pos.getX(), pos.getY(), pPosition->getFacing());
		}
		
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
	if (enabled) {
		if (mStateMachine->getSizeOfMachine() > 0) {
			mStateMachine->update();
		}
	}
	else {
		update_checkRespawn(elapsedTime);
	}
	if (poweredUp()) {
		update_checkPower(elapsedTime);
	}
	if(mHealth < 0){
		if(mType == TYPE::PLAYER){
			//End the game state
			gpGame->gameOver();
		}
		if(mType == TYPE::ENEMY){
			gpGame->getUnitManager()->deleteUnit(mID);
		}
	}
	if(mType == TYPE::ENEMY){
		gpGame->getGraphicsSystem()->writeText(*gpGame->getGraphicsSystem()->getBackBuffer(), *gpGame->getFont(), mpPositionComponent->getPosition().getX(), mpPositionComponent->getPosition().getY(), std::to_string(mID), BLACK_COLOR);
	}
}

Unit::TYPE Unit::onCollision(Unit * other)
{ //this would probably be cleaner with inheritance.
	if (!enabled) return mType;
	std::cout << "Collision " << mType << ", " << other->getType() << "\n";
	switch (other->getType()) {
		case NONE:
			break;
		case PLAYER:
			if (mType == COIN) {
				//TODO need to delay this
				gpGame->getUnitManager()->deleteUnit(mID);
			}
			else if (mType == MIGHTY_CANDY) {
				despawn(gpGame->getDataLoader()->getData(DataLoader::MIGHTY_CANDY_RESPAWN_TIME));
			}
			else if (mType == ENEMY) {
				//Check if the Player (other) is powered up
				if(other->poweredUp()){
					mHealth -= other->getDamageDone();
				}
			}
			break;
		case ENEMY:
			if (mType == PLAYER) {
				//Check if player (this unit) is powered up.
				if(!poweredUp()){
					mHealth -= other->getDamageDone();
				}
			}
			else if (mType == ENEMY_FOOD) {
				//TODO need to delay this.
				gpGame->getUnitManager()->deleteUnit(mID);
			}
			break;
		case ENEMY_FOOD:
			if (mType == ENEMY) {
				//eat food.
			}
			break;
		case MIGHTY_CANDY:
			if (mType == PLAYER) {
				powerUnitUp(
					static_cast<GameApp*>(gpGame)->getDataLoader()
					->getData(DataLoader::MIGHTY_CANDY_POWERUP_TIME)
				);
			}
			break;
		case COIN:
			if (mType == PLAYER) {
				auto game = static_cast<GameApp*>(gpGame);
				game->addToScore(
					game->getDataLoader()->getData(DataLoader::COIN_VALUE)
				);
			}
			break;
		default:
			break;
	}
	return mType;
}


void Unit::setType(TYPE unitType)
{
	mType = unitType;
	if (mType == PLAYER) {
		mMaxSpeed = gpGame->getDataLoader()->getData(DataLoader::PLAYER_VELOCITY);
		mDamageDone = gpGame->getDataLoader()->getData(DataLoader::PLAYER_DAMAGE);
	}
	else if (mType == ENEMY) {
		mMaxSpeed = gpGame->getDataLoader()->getData(DataLoader::ENEMY_VELOCITY);
		mDamageDone = gpGame->getDataLoader()->getData(DataLoader::ENEMY_DAMAGE);
	}
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

void Unit::powerUnitUp(float powerTime)
{
	poweredTimer = powerTime;
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