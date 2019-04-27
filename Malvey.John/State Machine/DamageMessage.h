#pragma once

#include "UnitManager.h"
#include "GameMessage.h"
class DamageMessage :public GameMessage
{
public:
	DamageMessage(const UnitID id, const int dmg);
	~DamageMessage();

	void process();

private:
	UnitID mUnitID;
	int mDMG;
};