#include "FollowPathSteering.h"

#include <cassert>
#include "Grid.h"
#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <math.h>
/*
FollowPathSteering::FollowPathSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: ArriveSteering(ownerID, targetLoc, targetID)
{
	mType = FOLLOW_PATH;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	//aStarPathfinder* aStarFinder = new aStarPathfinder(gpGame->getGridGraph());
	Node* start = gpGame->getGridGraph()->getNode(gpGame->getGrid()->getSquareIndexFromPixelXY(gpGame->getUnitManager()->getUnit(ownerID)->getPositionComponent()->getPosition().getX(), gpGame->getUnitManager()->getUnit(ownerID)->getPositionComponent()->getPosition().getY()));
	//std::cout << "Starting Goal: " << targetLoc.getX() << ", " << targetLoc.getY() << "\n";
	mpPath = gpGame->getPathfinder()->findPath(gpGame->getGridGraph()->getNode(gpGame->getGrid()->getSquareIndexFromPixelXY(targetLoc.getX(), targetLoc.getY())), start);
}

Steering* FollowPathSteering::getSteering()
{

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D diff;
	Vector2D pos = pOwner->getPositionComponent()->getPosition();
	Vector2D playerPos = gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition();
	Vector2D vec2player = pos - playerPos;
	float dist2player = vec2player.getLength();

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}
	Vector2D nextLoc;
	int numNodes = 0;
	if (mpPath)
	{
		numNodes = mpPath->getNumNodes();
	}

	if (numNodes > 0)
	{
		for (int i = 1; i < numNodes; i++)
		{
			if (gpGame->getGrid()->getValueAtIndex(mpPath->peekNode(i)->getId())/* getValueAtPixelXY(mpPath->peekNode(i)->getPos().getX(), mpPath->peekNode(i)->getPos().getY()))
			{
			//	aStarPathfinder* aStarFinder = new aStarPathfinder(gpGame->getGridGraph());
				//Node* start = gpGame->getGridGraph()->getNode(gpGame->getGrid()->getSquareIndexFromPixelXY(mpPath->peekNode(i - 1)->getPos().getX(), mpPath->peekNode(i - 1)->getPos().getY()));
				//mpPath = gpGame->getPathfinder()->findPath(gpGame->getGridGraph()->getNode(gpGame->getGrid()->getSquareIndexFromPixelXY(mTargetLoc.getX(), mTargetLoc.getY())), start);
				//FollowPathSteering* me = new FollowPathSteering(mOwnerID, mTargetLoc, mTargetID);
				Path* newPath = gpGame->getPathfinder()->findPath(mpPath->peekNode(mpPath->getNumNodes() - 1), mpPath->peekNode(0));
				mpPath = newPath;
				//return me->getSteering();
				break;
			
				
			}
			numNodes = mpPath->getNumNodes();
		}

		nextLoc = mpPath->peekNode(mpPath->getNumNodes() - 1)->getPos();

	}


	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
	}



	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	mTargetLoc = nextLoc;

	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	

	float angle = atan2(diff.getY(), diff.getX()) + 3.14159f / 2.0f;

	if (numNodes < 2)
	{
		data = arriveAndFace(pOwner);

		if ((pos - mTargetLoc).getLength() < 10.0f)
		{

			data.vel.normalize();
			data.vel *= 2;
			data.acc.normalize();
			data.acc *= 2;
			data.rotAcc = 0;
			data.rotVel = 0;
		}

	}

	else if ((pos - mTargetLoc).getLength() < 50.0f)
	{
		//std::cout << "Reached Node!\n";
		if(mpPath->getNumNodes() > 0)
			mpPath->getAndRemoveNextNode();
		//std::cout << "New Target: " << mTargetLoc.getX() << ", " << mTargetLoc.getY() << std::endl;
	}
	else
	{
		 data = arriveAndFace(pOwner);
	}


	mData = data;
	return this;

}
*/
