#include "ArriveSteering.h"

#include <cassert>

#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <math.h>

/*
	Modifying SeekSteering for my own nefarious purposes.
*/

ArriveSteering::ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Steering()
{
	mType = ARRIVE;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* ArriveSteering::getSteering()
{	
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	
	this->mData = arriveAndFace(pOwner);
	
	return this;
}


PhysicsData ArriveSteering::arriveAndFace(Unit* pOwner)
{
	static float SLOW_RADIUS = 100.0f;
	Vector2D diff;
	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();

	}


	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	Vector2D direction = diff;
	float distance = direction.getLength();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	//data.acc = diff;
	data.rotVel = 0.0f;
	data.rotAcc = 0.0f;
	//implementing arrive slowdown
	Vector2D targetVelocity;
	direction.normalize();
	if (distance > SLOW_RADIUS)
	{
		targetVelocity = direction * data.maxSpeed;
	}
	else
	{
		targetVelocity = direction * (data.maxSpeed * distance / SLOW_RADIUS);
	}

	data.acc = (targetVelocity - data.vel);
	data.acc /= .1f;

	if (data.acc.getLength() > data.maxAccMagnitude)
	{
		data.acc.normalize();
		data.acc *= data.maxAccMagnitude;
	}

	//position component to adjust the angle
	PositionComponent &pos = *pOwner->getPositionComponent();
	//data.rotVel = 1.0f;
	float pi = 3.14159f;
	float deltaAngle = 5.0f * (pi / 180.0f);

	float angle = fmod(atan2(diff.getY(), diff.getX()) + ( pi / 2), 2 *pi);

	
	if (distance > 5)
	{
		if (fmod(abs(pos.getFacing() - angle),2*pi) < 2.0f * deltaAngle)
		{
			pos.setFacing(angle);
		}
		else
		{
			
			//smoothly integrate angle until facing the target
			float currentAngle = fmod(pos.getFacing(), 2 * pi);
			
			//float relAngle = angle - pi;
			if (angle > currentAngle && fmod(angle - currentAngle, 2*pi) < pi)
			{
				//fmod(currentAngle - angle, 2*pi) < 3.14159f || 
				/*if ( angle - currentAngle > pi)
				{
					pos->modFacing(-deltaAngle);
				}
				else*/
				
				

				
					pos.modFacing(deltaAngle);
					/*if (angle > 3 * pi /2 && currentAngle < pi / 2)
					{
						pos->modFacing(-deltaAngle * 2);
					}*/
			}
			else if (fmod(currentAngle - angle, 2*pi) < pi)
			{/*
				if (currentAngle - angle > pi)
				{
					pos->modFacing(deltaAngle);
				}
				else*/
					pos.modFacing(-deltaAngle);
					/*if (angle < pi / 2 && currentAngle > 3 * pi / 2)
					{
						pos->modFacing(deltaAngle * 2);
					}*/
			}
			else
			{
				pos.modFacing(deltaAngle);

			}
			
			
			

			//pos->getFacing() - angle > 3.14159 ? pos->setFacing(pos->getFacing() - deltaAngle) : pos->setFacing(pos->getFacing() + deltaAngle);
			
			*pOwner->getPositionComponent() = (const PositionComponent&) pos;
		}

	}
	else
	{
		data.rotVel = 0.0f;
		data.vel = 0.0f;
		data.acc = 0.0f;
		data.rotAcc = 0.0f;
	}
	//delete pos;
	return data;
}
