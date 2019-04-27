#include <cassert>
#include "Steering.h"
#include "ArriveSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

ArriveSteering::ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc)
	: Steering()
{
	mType = Steering::ARRIVE;
	setOwnerID(ownerID);
	setTargetLoc(targetLoc);
}

Steering* ArriveSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	this->mData = data;
	return this;
}