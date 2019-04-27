#include <map>

#include "GameMessageManager.h"
#include "GameMessage.h"
#include "DeleteRandomMessage.h"
#include "HealthChangeMessage.h"

#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"
#include "ComponentManager.h"
#include "GraphicsSystem.h"
#include "GridGraph.h"
#include "Grid.h"
#include "ItemManager.h"
UnitID UnitManager::msNextUnitID = PLAYER_UNIT_ID + 1;

using namespace std;

UnitManager::UnitManager(Uint32 maxSize)
	:mPool(maxSize, sizeof(Unit))
{
}
UnitManager::~UnitManager()
{
	for (unsigned int i = 0; i <= mUnitMap.size(); i++)
	{
		deleteUnit(i);
	}
}
Unit* UnitManager::createUnit(Sprite* sprite, bool shouldWrap, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id)
{
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		pUnit = new (ptr)Unit(sprite);//placement new

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = msNextUnitID;
			msNextUnitID++;
		}

		//place in map
		mUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;

		//create some components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		ComponentID id = pComponentManager->allocatePositionComponent(posData,shouldWrap);
		pUnit->mPositionComponentID = id;
		pUnit->mpPositionComponent = pComponentManager->getPositionComponent(id);
		pUnit->mPhysicsComponentID = pComponentManager->allocatePhysicsComponent(pUnit->mPositionComponentID, physicsData);
		pUnit->mSteeringComponentID = pComponentManager->allocateSteeringComponent(pUnit->mPhysicsComponentID);

		//set max's
		pUnit->mMaxSpeed = MAX_SPEED;
		pUnit->mMaxAcc = MAX_ACC;
		pUnit->mMaxRotAcc = MAX_ROT_ACC;
		pUnit->mMaxRotVel = MAX_ROT_VEL;
		//if (pUnit->mpFSM != NULL)
		//{
		//	delete pUnit->mpFSM;
			//pUnit->mpFSM = NULL;	}
		if (pUnit->getID() != PLAYER_UNIT_ID)
		{
			pUnit->mpFSM = new StateMachine(theID);

		}
		else
			pUnit->mpFSM = new StateMachine;
	
	}
	if (pUnit != NULL)
	{
		//pUnit->setSteering(Steering::SEEK, Vector2D(rand() % gpGame->getGraphicsSystem()->getWidth(), rand() % gpGame->getGraphicsSystem()->getHeight()));
		pUnit->setSteering(Steering::WANDER, Vector2D(gpGame->getGraphicsSystem()->getWidth() / 2, gpGame->getGraphicsSystem()->getHeight() / 2));
	}
	return pUnit;
}


Unit* UnitManager::createPlayerUnit(Sprite* sprite, bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/)
{
	return createUnit(sprite, false, posData, physicsData, PLAYER_UNIT_ID);
}
Unit* UnitManager::getUnit(const UnitID& id) const
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::deleteUnit(const UnitID& id)
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

		//remove from map
		mUnitMap.erase(it);

		//remove components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		pComponentManager->deallocatePhysicsComponent(pUnit->mPhysicsComponentID);
		pComponentManager->deallocatePositionComponent(pUnit->mPositionComponentID);
		pComponentManager->deallocateSteeringComponent(pUnit->mSteeringComponentID);


		//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}

void UnitManager::deleteRandomUnit()
{
	if (mUnitMap.size() >= 1)
	{
		Uint32 target = rand() % mUnitMap.size();
		if (target == 0)//don't allow the 0th element to be deleted as it is the player unit
		{
			target = 1;
		}
		Uint32 cnt = 0;
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it, cnt++)
		{
			if (cnt == target)
			{
				deleteUnit(it->first);
				break;
			}
		}
	}
}

void UnitManager::drawAll() const
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->draw();
	}
}

void UnitManager::updateAll(float elapsedTime)
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		//change state if needed
		if ((it->second->getID() != gpGame->getUnitManager()->getPlayerUnit()->getID())&& gpGame->getUnitManager()->getUnit(it->second->getID())->getFSM() != NULL)
		{
			it->second->getFSM()->update();
			if (it->second->getFSM()->getCurrentState()->getName() != it->second->getFSM()->getPrevStateName())
			{
				if (it->second->getFSM()->getCurrentState()->getName() == HUNT_PLAYER)
				{
					//set steering to seek player for all units in the hunt state
					it->second->updateActiveSprite(HUNTER_ID);
					it->second->setSteering(Steering::ARRIVE, gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition(), PLAYER_UNIT_ID);
				}
				else if (it->second->getFSM()->getCurrentState()->getName() == SEEK_BOOST)
				{
					it->second->updateActiveSprite(BOOSTER_ID);
					if (gpGame->getItemManager()->itemExists(SPEED))
						it->second->setSteering(Steering::ARRIVE, gpGame->getItemManager()->getClosestItemOfType(it->second->getPositionComponent()->getPosition(), SPEED)->getPos());
					else if (gpGame->getItemManager()->itemExists(STRENGTH))
						it->second->setSteering(Steering::ARRIVE, gpGame->getItemManager()->getClosestItemOfType(it->second->getPositionComponent()->getPosition(), STRENGTH)->getPos());

				}
				else if (it->second->getFSM()->getCurrentState()->getName() == SEEK_HEALTH)
				{

					it->second->updateActiveSprite(HEALER_ID);
					if (gpGame->getItemManager()->itemExists(HEALTH))
						it->second->setSteering(Steering::ARRIVE, gpGame->getItemManager()->getClosestItemOfType(it->second->getPositionComponent()->getPosition(), HEALTH)->getPos());
				}
				else if (it->second->getFSM()->getCurrentState()->getName() == WANDER)
				{
 					it->second->updateActiveSprite(AI_ICON_SPRITE_ID);
					it->second->setSteering(Steering::WANDER, gpGame->getUnitManager()->getUnit(it->second->getID())->getPositionComponent()->getPosition(), it->second->getID());

				}
				
			}
			
		}

		//Check for units in damage radius
		checkDamage(it->second->getID());

		//Check if current unit is dead
		if (it->second->getHealth() <= 0)
		{
			GameMessage* pMessage = new DeleteRandomMessage(it->second->getID());
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		}



		//update position
		if(it->second->getID() == 0)
			it->second->update(elapsedTime * gpGame->mPlayerSpeedMult);
		else if (it->second->isSpeedBoosted())
		{
			it->second->update(elapsedTime * gpGame->mSpeedUpValue);
		}
		else
			it->second->update(elapsedTime);

	}
}

void UnitManager::updateSteering(Steering::SteeringType newSteer, Vector2D pos)
{

	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->setSteering(newSteer, pos);
	}
}

void UnitManager::checkDamage(const UnitID& id)
{
	float xDiff;
	float yDiff;

	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		if (it->second->getID() != id)
		{
			//if the damage dealer is the player || if the damage dealer isn't the player, but the unit being checked is
			if (id == 0 || (id != 0 && it->second->getID() == 0))
			{
				//if neither is the player, break
				if (id != 0 && it->second->getID() != 0)
					break;
				
			
				//get distance between damage dealer and iterator
				if (getUnit(id)->getPositionComponent()->getPosition().getX() > getUnit(it->second->getID())->getPositionComponent()->getPosition().getX())
					xDiff = getUnit(id)->getPositionComponent()->getPosition().getX() - getUnit(it->second->getID())->getPositionComponent()->getPosition().getX();
				else
					xDiff = getUnit(it->second->getID())->getPositionComponent()->getPosition().getX() - getUnit(id)->getPositionComponent()->getPosition().getX();

				if (getUnit(id)->getPositionComponent()->getPosition().getY() > getUnit(it->second->getID())->getPositionComponent()->getPosition().getY())
					yDiff = getUnit(id)->getPositionComponent()->getPosition().getX() - getUnit(it->second->getID())->getPositionComponent()->getPosition().getY();
				else
					yDiff = getUnit(it->second->getID())->getPositionComponent()->getPosition().getY() - getUnit(id)->getPositionComponent()->getPosition().getX();


				//if unit is in range of unit that is searching, deal damage
				if (xDiff <= getUnit(id)->getDamageRadius() || yDiff <= getUnit(id)->getDamageRadius())
				{
					GameMessage* pMessage = new HealthChangeMessage(it->second->getID(), (int)getUnit(id)->getDamage());
					MESSAGE_MANAGER->addMessage(pMessage, 0);
				}
				
			}
		}
	}

}