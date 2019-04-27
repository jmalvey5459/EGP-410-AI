#include "Item.h"
#include <assert.h>
#include "Game.h"
#include "GraphicsSystem.h"
#include "Component.h"
#include "PositionComponent.h"
#include "ComponentManager.h"

Item::Item(Sprite *sprite, Vector2D pos, ItemType type, float value, float duration): mpSprite(sprite)
{
	mPos.setX(pos.getX());
	mPos.setY(pos.getY());
	mType = type;
	mValue = value;
	mDuration = duration;
}

Item::~Item()
{

}


void Item::draw() const
{
	
	gpGame->getGraphicsSystem()->draw(*mpSprite, mPos.getX(), mPos.getY(), 0);
}


void Item::setType(ItemType type)
{
	mType = type;
}

void Item::setValue(float value)
{
	mValue = value;
}

void Item::setDuration(float duration)
{
	mDuration = duration;
}

Vector2D Item::getPos()
{
	return mPos;
}