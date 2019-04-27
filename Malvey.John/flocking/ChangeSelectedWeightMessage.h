#pragma once
#include "GameMessage.h"
#include "Game.h"

class ChangeSelectedWeightMessage : public GameMessage
{
	bool mPositive;
public:
	ChangeSelectedWeightMessage(bool isPos);
	~ChangeSelectedWeightMessage() {};

	void process();
};
