#include "WanderSteering.h"
#include "ArriveSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <cassert>
#include <ctime>

WanderSteering::WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: ArriveSteering(ownerID, targetLoc, targetID)
{
	mType = WANDER;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
	srand((unsigned int)time(0));

}

Steering* WanderSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D diff;
	Vector2D pos = pOwner->getPositionComponent()->getPosition();
	Vector2D playerPos = gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition();
	Vector2D vec2player = pos - playerPos;
	float dist2player = vec2player.getLength();

	

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (dist2player < 100.0f)
	{
		mTargetLoc = playerPos;
		data = arriveAndFace(pOwner);
	}
	else
	{
		diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

		float angle = atan2(diff.getY(), diff.getX()) +  3.14159f / 2.0f;

		if ((pos - mTargetLoc).getLength() < 5.0f)
		{
			int randomX = (int) (rand() % 200 + pOwner->getPositionComponent()->getPosition().getX() - 100);
			int randomY = (int) (rand() % 200 + pOwner->getPositionComponent()->getPosition().getY() - 100);
			randomY = randomY > 750 ? 750 : randomY;
			randomY = randomY < 0 ? 0 : randomY;
			randomX = randomX > 1000 ? 1000 : randomX;
			randomX = randomX < 0 ? 0 : randomX;

			mTargetLoc = (Vector2D(randomX, randomY));
		}
		else
		{
			data = arriveAndFace(pOwner);
		}
	}	
	
	mData = data;
	return this;
	
}