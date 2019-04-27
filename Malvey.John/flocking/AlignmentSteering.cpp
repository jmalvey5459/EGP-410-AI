#include "Steering.h"
#include "AlignmentSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "ComponentManager.h"
#include "PhysicsComponent.h"

AlignmentSteering::AlignmentSteering(const UnitID& ownerID)
	: Steering()
{

	mType = Steering::ALIGNMENT;
	setOwnerID(ownerID);
}

PhysicsData AlignmentSteering::computeAlignment()
{
	
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	Vector2D diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	Vector2D groupVel = pOwner->getPhysicsComponent()->getVelocity();
	float closeCount = 0;
	if (pOwner != NULL)
		{
		for (int i = 0; i < gpGame->getUnitManager()->getNumUnits(); i++)
		{
			if (gpGame->getUnitManager()->getUnit(i) != pOwner && gpGame->getUnitManager()->getUnit(i) != NULL)
			{
				if (pOwner->getPositionComponent()->getPosition().getLengthSquared() - gpGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition().getLengthSquared() < mAlignRadius)
				{
					groupVel += gpGame->getUnitManager()->getUnit(i)->getPhysicsComponent()->getVelocity();
					closeCount++;
				}
			}
		}
		//average all the facing data together
		if (closeCount != 0)
		{
			groupVel /= closeCount;

			groupVel.normalize();
			groupVel *= pOwner->getMaxAcc();
			pOwner->getPositionComponent()->setFacing(atan2(groupVel.getX(), -groupVel.getY()));

		}
		else
		{
			groupVel = pOwner->getPhysicsComponent()->getVelocity();
		}
	}
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	data.vel = groupVel;
	return data;
}

Steering* AlignmentSteering::getSteering()
{
	this->mData = computeAlignment();
	
	return this;
}