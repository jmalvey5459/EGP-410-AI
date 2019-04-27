#include "Transition.h"
#include "ItemManager.h"
#include <cmath>
Transition::Transition()
{
	mTargetState = INVALID_STATE;

}

Transition::Transition(UnitID owner, ConditionalType inCondition, StateName target)
{
	mConditionToMeet = inCondition;

	mTargetState = target;

	mOwner = owner;
}

Transition::~Transition()
{
	//delete mpConditionToMeet;
}

bool Transition::isTriggered()
{
	switch (mConditionToMeet) {
	case IF_PLAYER_IN_RANGE:
	{
		if (inRange(gpGame->getUnitManager()->getUnit(mOwner)->getPositionComponent()->getPosition(), gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition()))
			return true;
		break;
	}

	//COME BACK AND USE BOOST POSITION
	case IF_BOOST_IN_RANGE:
	{	
		for (auto iter = gpGame->getItemManager()->getItemMap().begin(); iter != gpGame->getItemManager()->getItemMap().end(); ++iter)
		{
		if (inRange(gpGame->getUnitManager()->getUnit(mOwner)->getPositionComponent()->getPosition(), iter->second->getPos()))
			return true;
		}
		
	break;
	}
	case	IF_BOOST_CLOSER_THAN_PLAYER:
	{	
		//if (boostIsCloser(gpGame->getUnitManager()->getUnit(mOwner)->getPositionComponent()->getPosition(), gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition(), findClosest(gpGame->getUnitManager()->getUnit(mOwner)->getPositionComponent()->getPosition()) ))
		//return true;
	break;
	}
	case	IF_LOW_HEALTH:
	{	
		if (!isHealthy())
		return true;
	break;
	}
	case	IF_NOTHING_IN_RANGE:
	{
		if (gpGame->getUnitManager()->getUnit(mOwner)->getFSM()->getPrevStateName() != WANDER)
		{
			if(!gpGame->getItemManager()->getItemMap().empty())
			if (nothingInRange(gpGame->getUnitManager()->getUnit(mOwner)->getPositionComponent()->getPosition(), gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition(), gpGame->getItemManager()->getClosestItem(gpGame->getUnitManager()->getUnit(mOwner)->getPositionComponent()->getPosition())->getPos()))
				return true;

		}
		break;
	}
	}
	return false;
}

StateName Transition::getTargetState()
{
	return mTargetState;
}

bool Transition::inRange(Vector2D sourcePos, Vector2D playerPos)
{
	float dist = sqrt(((sourcePos.getX() - playerPos.getX())*(sourcePos.getX() - playerPos.getX())) + ((sourcePos.getY() - playerPos.getY())*(sourcePos.getY() - playerPos.getY())));
	//std::cout << dist << " " << gpGame->getUnitManager()->getUnit(mOwner)->getScanRange() << "\n";
	if ( dist <=  gpGame->getUnitManager()->getUnit(mOwner)->getScanRange())
		return true;
	else
		return false;
}
bool Transition::isHealthy()
{
	//if enemey is half health or below, the enemy needs to seek healing
	if (gpGame->getUnitManager()->getUnit(mOwner)->getHealth() <= (gpGame->getUnitManager()->getUnit(mOwner)->getMaxHealth() / 2))
		return false;
	return true;
}
bool Transition::boostIsCloser(Vector2D sourcePos, Vector2D playerPos, Vector2D boostPos)
{
	//if the player is in range as well as at least one other boost, if the closest boost is closer than the player, seek the boost
	if (inRange(sourcePos, boostPos) && inRange(sourcePos, playerPos))
	{
		//get distance to nearest boost
		Vector2D diffB;
		diffB = boostPos - sourcePos;

		//get distance to player
		Vector2D diffP;
		diffP = playerPos - sourcePos;

		//if the boost is closer, return true
		if (diffB.getLength() < diffP.getLength())
			return true;

	}
	return false;
}
bool Transition::nothingInRange(Vector2D sourcePos, Vector2D playerPos, Vector2D boostPos)
{
	if (!inRange(sourcePos, boostPos) && !inRange(sourcePos, playerPos))
		return true;
	else
		return false;
}
Vector2D Transition::findClosest(Vector2D sourcePos)
{
	Vector2D closestItem = (3000000.0f,3000000.0f);
	for (auto it = gpGame->getItemManager()->getItemMap().begin(); it != gpGame->getItemManager()->getItemMap().end(); ++it)
	{
		if (inRange(gpGame->getUnitManager()->getUnit(mOwner)->getPositionComponent()->getPosition(), it->second->getPos()))
			if(it->second->getPos().getLength() < closestItem.getLength())
				closestItem = it->second->getPos();

		//std::cout << "Closest Item to " << gpGame->getUnitManager()->getUnit(mOwner)<< " " << gpGame->getUnitManager()->getUnit(mOwner)->getFSM()->getCurrentState()->getName()<< " is: " << it->second->getID() << "("<< it->second->getType() << ") " << std::endl;
	}
		return closestItem;	

}