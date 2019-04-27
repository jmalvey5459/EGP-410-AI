#include "FlockingSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "ComponentManager.h"
#include "PhysicsComponent.h"

struct PhysicsData;
class SeparationSteering;
class AlignmentSteering;
class CohesionSteering;

FlockingSteering::FlockingSteering(const UnitID& ownerID)
	: Steering()
{

	mType = Steering::FLOCK;
	setOwnerID(ownerID);

}
FlockingSteering::~FlockingSteering()
{
	
}

Steering*  FlockingSteering::getSteering()
{
	loadWeights();
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	SeparationSteering sepData(mOwnerID);
	AlignmentSteering alignData(mOwnerID);
	CohesionSteering cohData(mOwnerID);
	
	//slap all these puppies together
	data = data + (sepData.computeSeparation() * mSeparateWeight) + (cohData.computeCohesion() * mCohesionWeight) + (alignData.computeAlignment() * mAlignWeight);

	this->mData = data;
	return this;
}


void FlockingSteering::loadWeights()
{

	mSeparateWeight = gpGame->mSeparateWeight;
	mAlignWeight = gpGame->mAlignWeight;
	mCohesionWeight = gpGame->mCohesionWeight;

}