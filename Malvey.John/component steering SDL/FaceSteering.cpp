/*class Face (Align):
10
19 # Put the target together
20 Align.target = explicitTarget
21 Align.target.orientation = atan2(-direction.x, direction.z)
22
23 # 2. Delegate to align
24 return Align.getSteering()*/
#include "Steering.h"
#include "FaceSteering.h"
#include <Trackable.h>
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
//I'm just going to put the one important line of code into arrive wander and chase
FaceSteering::FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc)
{
	//mType = Steering::FACE;
	setOwnerID(ownerID);
	setTargetLoc(targetLoc);
}

Steering* FaceSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//if (diff.getLength != 0.0f ) 
	{
		pOwner->getPositionComponent()->setFacing(atan2(-diff.getX(), diff.getY()));
	}
	this->mData = data;
	return this;
}