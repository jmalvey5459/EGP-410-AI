#pragma once
#include "GameMessage.h"

class CreateRandomMessage: public GameMessage
{
public:
	CreateRandomMessage():GameMessage(CREATE_RANDOM_MESSAGE) {};
	~CreateRandomMessage() {};

	void process();
};