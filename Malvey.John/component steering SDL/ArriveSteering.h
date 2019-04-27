#pragma once
#include "Steering.h"
#include <Trackable.h>
class ArriveSteering : public Steering
{
public:
	ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);
private:
	virtual Steering* getSteering();
};