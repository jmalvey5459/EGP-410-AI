#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <MemoryPool.h>
#include <map>
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "Unit.h"
#include "Steering.h"
#include "Game.h"
class Game;
class Steering;
class Unit;
class Sprite;
struct PositionData;
struct PhysicsData;
class ItemManager;
class UnitManager;
const UnitID PLAYER_UNIT_ID = 0;


class UnitManager : public Trackable
{
public:
	UnitManager(Uint32 maxSize);
	~UnitManager();

	Unit* createUnit(Sprite* sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA, const UnitID& id = INVALID_UNIT_ID);
	Unit* createPlayerUnit(Sprite* sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA);
	//Unit* createRandomUnit(const Sprite& sprite);

	Unit* getUnit(const UnitID& id) const;
	void deleteUnit(const UnitID& id);
	void deleteRandomUnit();

	void checkDamage(const UnitID& id);
	

	void drawAll() const;
	void updateAll(float elapsedTime);

	void updateSteering(Steering::SteeringType newSteer, Vector2D pos);

	Unit* getPlayerUnit() const { return getUnit(PLAYER_UNIT_ID); };

private:
	static UnitID msNextUnitID;
	MemoryPool mPool;
	std::map<UnitID, Unit*> mUnitMap;

	friend class ItemManager;
};

