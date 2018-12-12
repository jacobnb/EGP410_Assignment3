#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <limits>
#include <Vector2D.h>

#include "Component.h"
#include "PositionComponent.h"
#include "Sprite.h"
#include "Steering.h"
//#include "CircularQueue.h"
//#include "Transaction.h"
//#include "TransactionProcessor.h"

class PhysicsComponent;
class SteeringComponent;
class UnitManager;
class StateMachine;

const Uint32 DEFAULT_QUEUE_CAPACITY = 8;


//class Unit : public TransactionProcessor
class Unit : public Trackable
{
public:
	enum TYPE {
	NONE = -1,
	PLAYER = 0,
	ENEMY,
	ENEMY_FOOD,
	MIGHTY_CANDY,
	COIN,
	};
	void setAltSprite(Sprite* altSprite) { mpAltSprite = altSprite; };
	void draw() const;
	void setActive(bool isActive) { enabled = isActive; }
	bool isActive() const { return enabled; };
	float getFacing() const;
	void update(float elapsedTime);
	TYPE onCollision(Unit* other);
	float getCollisionRadius() const { return mCollisionRadius; };
	void setCollisionRadius(float newRadius) { mCollisionRadius = newRadius; };

	TYPE getType() const { return mType; };
	void setType(TYPE unitType);
	PositionComponent* getPositionComponent() const;
	PhysicsComponent* getPhysicsComponent() const;
	SteeringComponent* getSteeringComponent() const;
	StateMachine* getStateMachine() const { return mStateMachine; };
	float getMaxAcc() const { return mMaxAcc; };
	float getMaxSpeed() const { return mMaxSpeed; };
	float getMaxRotAcc() const { return mMaxRotAcc; };
	float getMaxRotVel() const { return mMaxRotVel; };
	void setShowTarget(bool val) { mShowTarget = val; };
	int getHealth() const {return mHealth;};
	void setHealth(int health) {mHealth = health;};
	bool poweredUp() const { return poweredTimer > 0; };
	void powerUnitUp(float powerTime);
	int getDamageDone() const {return mDamageDone;};
	void setDamageDone(int damage){mDamageDone = damage;};
	void setNextPos(Vector2D pos){nextPos = pos;};
	Vector2D getNextPos() const {return nextPos;};
	bool getEnabled() const { return enabled;};

	
	UnitID GetID() const { return mID; };

	void setSteering(Steering::SteeringType type, Vector2D targetLoc = ZERO_VECTOR2D, UnitID targetUnitID = INVALID_UNIT_ID);
	void setSteering(Steering::SteeringType type, std::vector<Vector2D> targetLoc, UnitID targetUnitID = INVALID_UNIT_ID);
	
	bool isFinished = false;
	
private:
	UnitID mID;
	TYPE mType;
	bool enabled;
	ComponentID mPhysicsComponentID;
	ComponentID mPositionComponentID;
	ComponentID mSteeringComponentID;
	PositionComponent* mpPositionComponent = NULL;
	StateMachine* mStateMachine;
	Sprite mSprite;
	Sprite* mpAltSprite;
	float mMaxAcc;
	float mMaxSpeed;
	float mMaxRotAcc;
	float mMaxRotVel;
	bool mShowTarget;
	int mHealth;
	float poweredTimer;

	int mDamageDone;
	//replace this if time.
	float mCollisionRadius = 10; 
	float disabledTimer;

	Vector2D nextPos = NULL;

	void update_checkRespawn(float elapsedTime);
	void update_checkPower(float elapsedTime);

	Unit(const Sprite& sprite);
	virtual ~Unit();
	void despawn(float spawnTime); //despawn and set respawn timer.
	Unit(Unit&);//invalidate copy constructor
	void operator=(Unit&);//invalidate assignment operator

	friend class UnitManager;
};
