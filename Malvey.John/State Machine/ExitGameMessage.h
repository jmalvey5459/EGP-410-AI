#pragma once

#include "GameMessage.h"

class ExitGameMessage : public GameMessage
{
public:
	ExitGameMessage() :GameMessage(EXIT_GAME_MESSAGE) {};
	~ExitGameMessage() {};
	void process();
};