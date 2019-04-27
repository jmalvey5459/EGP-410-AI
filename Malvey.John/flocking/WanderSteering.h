#pragma once
#include "Steering.h"
#include <Trackable.h>

class WanderSteering : public Steering
{
public:
	WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc,const UnitID& targetID);
private:


	virtual Steering* getSteering();
};