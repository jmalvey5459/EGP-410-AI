#pragma once

#include "GameMessage.h"

class HealthChangeMessage : public GameMessage
{
public:
	HealthChangeMessage(const UnitID unitID, const int change );
	~HealthChangeMessage();

	void process();

private:
	UnitID mUnitID;
	int mChange;
};