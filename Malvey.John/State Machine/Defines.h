#pragma once

#include <string>
#include <deanlibdefines.h>

typedef int IDType;
typedef std::string IDDescriptor;
typedef std::string GraphicsBufferID;

const IDType INVALID_ID = -1;

const IDType HIGHEST_ID = 9999;

#define MESSAGE_MANAGER gpGame->getMessageManager()
#define GRAPHICS_SYSTEM gpGame->getGraphicsSystem()

typedef Uint32 UnitID;
typedef Uint32 ItemID;

const UnitID INVALID_UNIT_ID = UINT_MAX;

enum StateName
{
	INVALID_STATE = -1,
	HUNT_PLAYER,
	SEEK_BOOST,
	SEEK_HEALTH,
	WANDER,
};

enum ConditionalType
{
	INVALID_TYPE = -1,
	IF_PLAYER_IN_RANGE,
	IF_BOOST_IN_RANGE,
	IF_BOOST_CLOSER_THAN_PLAYER,
	IF_LOW_HEALTH,
	IF_NOTHING_IN_RANGE,
};

enum ItemType {
	INVALID_ITEM = -1,
	HEALTH,
	STRENGTH,
	SPEED
};