#include <cassert>

#include "Steering.h"
#include "SeekSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <math.h>


SeekSteering::SeekSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::SEEK;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* SeekSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	

	Vector2D playerPos = pOwner->getPositionComponent()->getPosition();
	
	if (playerPos.getX() > 882.0f)
		playerPos.setX(875.0f);
	if (playerPos.getX() < 86.0f)
		playerPos.setX( 90.0f);
	diff = playerPos - mTargetLoc;

	PhysicsData pData = pOwner->getPhysicsComponent()->getData();


	pData.acc = diff;
	this->mData = pData;
	return this;
}

