#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <limits>
#include <Vector2D.h>

#include "StateMachine.h"

#include "Component.h"
#include "PositionComponent.h"
#include "Sprite.h"
#include "Steering.h"
//#include "CircularQueue.h"
//#include "Transaction.h"
//#include "TransactionProcessor.h"
#include "StateMachine.h"

class PhysicsComponent;
class SteeringComponent;
class Sprite;
class UnitManager;
class StateMachine;

const Uint32 DEFAULT_QUEUE_CAPACITY = 8;

//class Unit : public TransactionProcessor
class Unit : public Trackable
{
public:
	void draw() const;
	float getFacing() const;
	void update(float elapsedTime){};

	PositionComponent* getPositionComponent() const;
	PhysicsComponent* getPhysicsComponent() const;
	SteeringComponent* getSteeringComponent() const;
	float getMaxAcc() const { return mMaxAcc; };
	float getMaxSpeed() const { return mMaxSpeed; };
	float getMaxRotAcc() const { return mMaxRotAcc; };
	float getMaxRotVel() const { return mMaxRotVel; };
	void setShowTarget(bool val) { mShowTarget = val; };
	UnitID getID();
	void setSteering(Steering::SteeringType type, Vector2D targetLoc = ZERO_VECTOR2D, UnitID targetUnitID = INVALID_UNIT_ID);
	

	float getScanRange() { return mScanRange; };
	float getHealth() const { return mHP; };
	float getMaxHealth() const { return mMaxHP; };
	float getDamageRadius() const { return mDamageRadius;};
	float getDamage()const { return mDamage; };

	void setHealth(int change);

	StateMachine* getFSM() const { return mpFSM; };

	void updateActiveSprite(IDType spriteID);

	void toggleDamageBoost();
	void toggleSpeedBoost();

	bool isDamageBoosted();
	bool isSpeedBoosted();

private:
	UnitID mID;
	ComponentID mPhysicsComponentID;
	ComponentID mPositionComponentID;
	ComponentID mSteeringComponentID;
	PositionComponent* mpPositionComponent = NULL;
	Sprite* mpSprite;

	float mMaxAcc;
	float mMaxSpeed;
	float mMaxRotAcc;
	float mMaxRotVel;
	bool mShowTarget;

	StateMachine* mpFSM;

	float mMaxHP;
	float mHP;

	float mScanRange;
	float mDamage;
	float mDamageRadius;

	bool applyDamageBonus = false;
	bool applySpeedBonus = false;

	Unit(Sprite* sprite);
	virtual ~Unit();

	Unit(Unit&);//invalidate copy constructor
	void operator=(Unit&);//invalidate assignment operator

	friend class UnitManager;
};
