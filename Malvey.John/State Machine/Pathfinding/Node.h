#pragma once

#include <Trackable.h>
#include "Vector2D.h"
const int BAD_NODE_ID = -1;
#define NODE_ID int

class Node:public Trackable
{
public:
	Node( const NODE_ID& id );
	Node();
	~Node();

	const NODE_ID& getId() const {return mId;};
	void setPos(Vector2D nodePos);
	Vector2D getPos();
private:
	const NODE_ID mId;
	Vector2D mPos;
};