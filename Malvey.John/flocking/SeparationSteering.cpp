#include <cassert>

#include "Steering.h"
#include "SeparationSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

SeparationSteering::SeparationSteering(const UnitID& ownerID)
	: Steering()
{	
	mType = Steering::SEPARATE;
	setOwnerID(ownerID);
	
	
}
PhysicsData SeparationSteering::computeSeparation()
{	
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D diff = pOwner->getPositionComponent()->getPosition() - mTargetLoc;
		if (pOwner != NULL)
		{
			float closeCount = 0;
			for (int i = 0; i < gpGame->getUnitManager()->getNumUnits(); i++)
			{
				if (gpGame->getUnitManager()->getUnit(i) != pOwner && gpGame->getUnitManager()->getUnit(i) != NULL)
				{
					if (pOwner->getPositionComponent()->getPosition().getLengthSquared() - gpGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition().getLengthSquared() < mSeparationRadius)
					{
						diff += (gpGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition());
						closeCount++;
					}
				}
			}

			if (closeCount > 0)
			{
				diff /= closeCount;
				diff *= -1.0f;
			}
			diff.normalize();
			diff *= pOwner->getMaxAcc();
		}

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	data.vel = diff;
	return(data);
}

Steering* SeparationSteering::getSteering()
{
	
	this->mData = computeSeparation();
	return this;
}

