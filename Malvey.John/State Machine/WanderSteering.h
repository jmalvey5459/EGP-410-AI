#pragma once
#include "Steering.h"
#include "ArriveSteering.h"
#include "Trackable.h"

class WanderSteering : public ArriveSteering
{
public:
	WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);

protected:
	virtual Steering* getSteering();
};
