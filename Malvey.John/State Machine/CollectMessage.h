#pragma once
#include "GameMessage.h"
#include "UnitManager.h"
class CollectMessage : public GameMessage
{
public:
	CollectMessage(const ItemID itemID, Unit* unitCollecting) :GameMessage(COLLECT_MESSAGE), mItemID(itemID)
		, mpUnit(unitCollecting) {};
	~CollectMessage() {};

	void process();
private:
	ItemID mItemID;
	Unit* mpUnit;
};