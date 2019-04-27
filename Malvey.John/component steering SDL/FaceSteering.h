#pragma once
#include "Steering.h"
#include <Trackable.h>
class FaceSteering: public Steering
{
public:
		FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc);
private:
	virtual Steering* getSteering();
};