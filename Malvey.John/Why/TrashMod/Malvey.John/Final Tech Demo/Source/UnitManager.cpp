#include <map>

#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"
#include "ComponentManager.h"
#include "GraphicsSystem.h"
#include "vector"
UnitID UnitManager::msNextUnitID = PLAYER_UNIT_ID + 1;

using namespace std;

UnitManager::UnitManager(Uint32 maxSize)
	:mPool(maxSize, sizeof(Unit))
{
}

Unit* UnitManager::createUnit(const Sprite& sprite, bool shouldWrap, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id)
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
		pUnit->mMaxSpeed = 1500.0f;
		pUnit->mMaxAcc = 500.0f;
		pUnit->mMaxRotAcc = MAX_ROT_ACC;
		pUnit->mMaxRotVel = MAX_ROT_VEL;

	}

	return pUnit;
}


Unit* UnitManager::createPlayerUnit(const Sprite& sprite, bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/)
{
	return createUnit(sprite, false, posData, physicsData, PLAYER_UNIT_ID);
}


Unit* UnitManager::createRandomUnit(const Sprite& sprite)
{

	int posX = rand() % gpGame->getGraphicsSystem()->getWidth();
	int posY = rand() % gpGame->getGraphicsSystem()->getHeight();

	Unit* pUnit = createUnit(sprite, false, PositionData(mFireSpawnLoc, 4.3f), PhysicsData(Vector2D(-15, -15), Vector2D(0.1f, 0.1f), 0.1f, 0.05f));
	if (pUnit != NULL)
	{
		return pUnit;
	}
	return NULL;
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
		if (target <= 1)//don't delete the player or the goblin
		{
			target = 2;
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
	std::vector<UnitID> toDelete;
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		if (it->second->mID > 1)
		{

			//std::cout << getUnit(2)->getPositionComponent()->getPosition().getX() << ", " << getUnit(2)->getPositionComponent()->getPosition().getY() << "\n";

			if (it->second->getPositionComponent()->getPosition().getY() > 608)
				toDelete.push_back(it->second->mID);
			else
				it->second->update(elapsedTime);

		}
		else
			it->second->update(elapsedTime);
	}
	for (unsigned int i = 0; i < toDelete.size(); i++)
	{
		deleteUnit(toDelete[i]);
	}
	toDelete.clear();
}
