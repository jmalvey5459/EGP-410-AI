#pragma once
#include "Defines.h"
#include <Trackable.h>
#include <DeanLibDefines.h>
#include <limits>
#include <Vector2D.h>

#include "Component.h"
#include "PositionComponent.h"
#include "Sprite.h"

class Sprite;

class Item
{
public:

	Item(Sprite *sprite, Vector2D pos, ItemType type, float value, float duration);
	~Item();

	void draw() const;

	void setType(ItemType type);
	void setValue(float value);
	void setDuration(float duration);
	
	ItemType getType() { return mType; };
	float getValue() { return mValue; };
	float getDuration() { return mDuration; };
	ItemID getID() { return mID; };
	Vector2D getPos();

private:


	ItemID mID;

	Vector2D mPos;

	Sprite* mpSprite;

	ItemType mType;
	float mValue;
	float mDuration;
	friend class ItemManager;
};