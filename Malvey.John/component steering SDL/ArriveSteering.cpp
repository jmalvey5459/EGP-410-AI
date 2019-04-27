#include "ArriveSteering.h"
#include <cassert>
#include "Steering.h"
#include <Trackable.h>
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

ArriveSteering::ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)//only difference between this and the provided seek steering is the lack of whether it should flee
	: Steering()
{
	mType = Steering::ARRIVE;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}
Steering* ArriveSteering::getSteering()
{
	/* 
50 # Check if the acceleration is too fast
51 if steering.linear.length() > maxAcceleration:
52 steering.linear.normalize()
53 steering.linear *= maxAcceleration
54
55 # Output the steering
56 steering.angular = 0
57 return steering*/
	Vector2D diff;
	float slowRadius = 50;
	float targetRadius = 7;
	float targetSpeed = 0.0f;
	Vector2D targetVelocity = (0.0f,0.0f);
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?

	if (mTargetID != INVALID_UNIT_ID)
	{
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}
	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	pOwner->getPositionComponent()->setFacing(atan2(diff.getX(), -diff.getY()));
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (diff.getLengthSquared() < targetRadius)
	{
		data.vel = 0;
	}

	else if (diff.getLength() >  slowRadius)
	{
		diff.normalize();
		diff *= pOwner->getMaxAcc();
		
		data.acc = diff;
		data.rotVel = 1.0f;
	}
	else
	{ 		
		//diff.normalize();
		targetSpeed = data.maxSpeed * diff.getLength() / slowRadius;
		targetVelocity = diff;
		targetVelocity.normalize();
		targetVelocity *= targetSpeed;
		//data.vel = data.maxSpeed * diff / slowRadius;
		data.acc = targetVelocity - data.vel;
		data.acc /= 0.1f;
		
	}
	
	this->mData = data;
	return this;
}