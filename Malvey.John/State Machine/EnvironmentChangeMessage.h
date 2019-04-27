#pragma once

#include "GameMessage.h"
#include "Vector2D.h"

class Vector2D;

class EnvironmentChangeMessage :public GameMessage
{
public:
	EnvironmentChangeMessage(const Vector2D& pos);
	~EnvironmentChangeMessage();

	void process();

private:
	Vector2D mPos;
};