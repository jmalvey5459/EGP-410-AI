#pragma once
#include "Path.h"
#include <Trackable.h>
#include "Steering.h"
#include "PhysicsComponent.h"
#include "aStarPathfinder.h"
#include "ArriveSteering.h"

class Unit;

class FollowPathSteering : public ArriveSteering
{
public:

//	FollowPathSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID);
	


private:
	Path* mpPath;
	Node *mpTo, *mpFrom;

protected:
	//virtual Steering* getSteering();
};