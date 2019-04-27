#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "PhysicsComponent.h"

class Unit;

class ArriveSteering : public Steering
{
public:
	ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);

	PhysicsData arriveAndFace(Unit * pOwner);

protected:
	virtual Steering* getSteering();
};