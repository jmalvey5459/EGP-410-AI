#pragma once
#include "GameMessage.h"
#include "Defines.h"|
#include "Vector2D.h"
class Vector2D;

class CreateItemMessage : public GameMessage
{
	
public:
	CreateItemMessage(ItemType type) :GameMessage(CREATE_ITEM_MESSAGE), mType(type) {};
	~CreateItemMessage() {};
	void process();
private:
	ItemType mType;
	Vector2D mPos;

};