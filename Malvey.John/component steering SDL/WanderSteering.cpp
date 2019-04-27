/*
24 # 1. Calculate the target to delegate to face
25
26 # Update the wander orientation
27 wanderOrientation += randomBinomial() * wanderRate
28
29 # Calculate the combined target orientation
30 targetOrientation = wanderOrientation +
31 character.orientation
32
33 # Calculate the center of the wander circle
34 target = character.position +
35 wanderOffset * character.orientation.asVector()
36
37 # Calculate the target location
38 target += wanderRadius * targetOrientation.asVector()
39
40 # 2. Delegate to face
41 steering = Face.getSteering()
42
43 # 3. Now set the linear acceleration to be at full
44 # acceleration in the direction of the orientation
45 steering.linear = maxAcceleration *
46 character.orientation.asVector()
47
48 # Return it
49 return steering*/
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "WanderSteering.h"
WanderSteering::WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
{
	mType = Steering::WANDER;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}
Steering* WanderSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	float chaseRadius = 75.0f;
	Vector2D diff;
	/*if (((mTargetLoc.getX() >= pOwner->getPositionComponent()->getPosition().getX() + chaseRadius) || mTargetLoc.getX() < pOwner->getPositionComponent()->getPosition().getX - chaseRadius) && ((mTargetLoc.getY() >= pOwner->getPositionComponent()->getPosition().getY() + chaseRadius) || mTargetLoc.getY() < pOwner->getPositionComponent()->getPosition().getY - chaseRadius))
	{

	}*/ ///WHY AM I LIKE THIS? THIS IS DUMB
	//check the chase 
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Vector2D playerLoc = gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition();
	float slowRadius = 50;
	float targetRadius = 7;
	float targetSpeed = 0.0f;
	Vector2D targetVelocity = (0.0f, 0.0f);
	if ((playerLoc - pOwner->getPositionComponent()->getPosition()).getLength() < chaseRadius)
	{
		mTargetLoc = playerLoc;
		diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

		pOwner->getPositionComponent()->setFacing(atan2(diff.getX(), -diff.getY()));
		

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

	}
	else
	{
		if ((pOwner->getPositionComponent()->getPosition() - mTargetLoc).getLength() < 7.0f)
		{
			mTargetLoc = Vector2D(rand()%150 + pOwner->getPositionComponent()->getPosition().getX() - 75 , rand()%150 + pOwner->getPositionComponent()->getPosition().getY()- 75);
		}
		else
		{
			
			diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

			pOwner->getPositionComponent()->setFacing(atan2(diff.getX(), -diff.getY()));

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

		}
	}
	//arrive steering for hunting the player
	
	this->mData = data;
	return this;
}