#include "Steering.h"
#include "CohesionSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "ComponentManager.h"
#include "PhysicsComponent.h"
CohesionSteering::CohesionSteering(const UnitID& ownerID)
	: Steering()
{

	mType = Steering::COHESION;
	setOwnerID(ownerID);
} 

PhysicsData CohesionSteering::computeCohesion()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	if (pOwner != NULL)
	{
		float closeCount = 0;
		float groupFace = pOwner->getFacing();

		for (int i = 0; i < gpGame->getUnitManager()->getNumUnits(); i++)
		{
			if (gpGame->getUnitManager()->getUnit(i) != pOwner && gpGame->getUnitManager()->getUnit(i) != NULL)
			{
				if (pOwner->getPositionComponent()->getPosition().getLengthSquared() - gpGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition().getLengthSquared() < mCohesiveRadius)
				{
					diff += gpGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition();
					closeCount++;
				}
			}
		}
		//get avg positon of flock
		if (closeCount > 0)
			diff /= closeCount;


		diff.normalize();
		diff *= pOwner->getMaxAcc();
	}


	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	data.vel = diff;

	return data;
}

Steering* CohesionSteering::getSteering()
{
	this->mData = computeCohesion();
	return this;
}