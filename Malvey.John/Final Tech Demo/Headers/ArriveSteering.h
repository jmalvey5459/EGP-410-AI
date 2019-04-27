#pragma once
#include <Trackable.h>
#include "Steering.h"

class ArriveSteering : public Steering
{
public:
	ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc);

protected:
	virtual Steering* getSteering();
	float mTargetRadius = 15.0f;
	float mSlowRadius = 40.0f;
};