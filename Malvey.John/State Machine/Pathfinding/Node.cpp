#include "Node.h"

Node::Node()
:mId(BAD_NODE_ID)
{
}

Node::Node( const NODE_ID& id )
:mId(id)
{
}

Node::~Node()
{
}
void Node::setPos(Vector2D nodePos)
{
	mPos = nodePos;
}
Vector2D Node::getPos()
{
	return mPos;
}