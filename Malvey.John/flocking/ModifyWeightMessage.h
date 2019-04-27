#pragma once
#include "GameMessage.h"
#include "Game.h"

class ModifyWeightMessage : public GameMessage
{
	bool mPositive;
public:
	ModifyWeightMessage(bool isPos);
	~ModifyWeightMessage() {};

	void process();
};
