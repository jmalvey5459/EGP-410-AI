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

Unit::Unit(Sprite* sprite) 
	:mpSprite(sprite)
	,mPositionComponentID(INVALID_COMPONENT_ID)
	,mPhysicsComponentID(INVALID_COMPONENT_ID)
	,mSteeringComponentID(INVALID_COMPONENT_ID)
	,mShowTarget(false)
{
	if (mID == PLAYER_UNIT_ID)
	{
		 mMaxHP = gpGame->mPlayerHealth;
		 mHP = gpGame->mPlayerHealth;
		 mDamage = gpGame->mPlayerDamage;
		 mDamageRadius = gpGame->mPlayerDamageRadius;
	}
	else
	{
		mMaxHP = gpGame->mEnemyHealth;
		mHP = gpGame->mEnemyHealth;
		mScanRange = gpGame->mEnemySeekRadius;
		mDamage = gpGame->mEnemyDamage;
		mDamageRadius = gpGame ->mEnemyDamageRadius;
	}
}

Unit::~Unit()
{
	if (mpFSM != NULL && this->getID() != PLAYER_UNIT_ID)
	{
		mpFSM->deleteAllStates();
		delete mpFSM;
		mpFSM = NULL;
	}
	else if (mpFSM != NULL)
	{
		mpFSM->deleteAllStates();
		delete mpFSM;
		mpFSM = NULL;
	}
}

void Unit::draw() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	const Vector2D& pos = pPosition->getPosition();
	gpGame->getGraphicsSystem()->draw(*mpSprite, pos.getX(), pos.getY(), pPosition->getFacing());

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

UnitID Unit::getID()
{
	return mID;
}

float Unit::getFacing() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	return pPosition->getFacing();
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
/*
/*void Unit::setSteering(Steering::SteeringType type, Vector2D targetLoc /*= ZERO_VECTOR2D, ItemID targetItemID /*= INVALID_UNIT_ID)
{
	SteeringComponent* pSteeringComponent = getSteeringComponent();
	if (pSteeringComponent != NULL)
	{
		pSteeringComponent->setData(SteeringData(type, targetLoc, mID, targetItemID));
	}
}**/
void Unit::updateActiveSprite(IDType spriteID)
{
	if(this->getID()!= 0)
		mpSprite = gpGame->getSpriteManager()->getSprite(spriteID);
}
void Unit::setHealth(int change)
{
	mHP += change;
	if (mHP > mMaxHP)
		mHP = mMaxHP;
}


void Unit::toggleDamageBoost()
{
	applyDamageBonus = !applyDamageBonus;
}
void Unit::toggleSpeedBoost()
{
	applySpeedBonus = !applySpeedBonus;
}

bool Unit::isDamageBoosted()
{
	return applyDamageBonus;
}
bool Unit::isSpeedBoosted()
{
	return applySpeedBonus;
}