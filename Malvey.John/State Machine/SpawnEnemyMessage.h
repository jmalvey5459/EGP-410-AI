#pragma once

#include "GameMessage.h"

class SpawnEnemyMessage : public GameMessage
{
public:
	SpawnEnemyMessage() :GameMessage(SPAWN_MESSAGE) {};
	~SpawnEnemyMessage() {};
	void process();
};