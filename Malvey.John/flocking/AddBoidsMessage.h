#pragma once
#include "GameMessage.h"
#include "Game.h"
#include "UnitManager.h"
#include "GraphicsSystem.h"

class AddBoidsMessage : public GameMessage
{

public:
	AddBoidsMessage() :GameMessage(ADD_BOIDS) {};
	~AddBoidsMessage() {};

	void process();

};