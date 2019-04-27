#pragma once
#ifndef TRANSITION_H
#define TRANSITION_H


#include "Game.h"
/*
#include "UnitManager.h"
#include "Unit.h"*/
#include "Defines.h"
#include <Vector2D.h>
#include "UnitManager.h"
class ItemManager;
class Transition :public Trackable
{
public:

	Transition();
	Transition(UnitID owner, ConditionalType inCondition, StateName target);
	~Transition();
	//test if conditions are met
	bool isTriggered();

	//get the target state if transition is triggered
	StateName getTargetState();


	bool inRange(Vector2D sourcePos, Vector2D playerPos);
	bool isHealthy();
	bool boostIsCloser(Vector2D sourcePos, Vector2D playerPos, Vector2D boostPos);
	bool nothingInRange(Vector2D sourcePos, Vector2D playerPos, Vector2D boostPos);
	Vector2D findClosest(Vector2D sourcePos); //returns position of closest object
private:
	ConditionalType mConditionToMeet;
	StateName mTargetState;
	UnitID mOwner;
};
#endif // !TRANSITION_H