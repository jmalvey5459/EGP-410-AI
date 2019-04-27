#pragma once
#include "GameMessage.h"
#include "Game.h"
#include "UnitManager.h"
#include "GraphicsSystem.h"

class DeleteBoidMessage : public GameMessage
{

public:
	DeleteBoidMessage() :GameMessage(REMOVE_BOID) {};
	~DeleteBoidMessage() {};

	void process();

};