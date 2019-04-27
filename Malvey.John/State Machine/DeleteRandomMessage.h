#pragma once
#include "GameMessage.h"
#include "UnitManager.h"
class DeleteRandomMessage : public GameMessage
{
public :
	DeleteRandomMessage(const UnitID unitID);
	~DeleteRandomMessage() {};

	void process();
private:
	UnitID mUnitID;
};