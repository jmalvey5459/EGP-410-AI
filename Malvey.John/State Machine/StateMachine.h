#pragma once
#ifndef STATEMACHINE_H
#define STATEMACHINE_H



#include <vector>
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

#include "State.h"
class State;


class StateMachine :public Trackable
{
public:

	StateMachine();
	StateMachine(UnitID& owner);
	~StateMachine();

	void deleteAllStates();

	void init();

	State* getCurrentState();
	StateName getPrevStateName();
	void update();



	//new variables to disallow state changes until the main task of the state has been completed at least once
	bool isCurrentStateObjective()const { return mCurrentStateIsAnObjective; };
	bool isObjectiveComplete() const { return mObjectiveReached; };

	void objectiveSuccess();

private: 


	bool mCurrentStateIsAnObjective = false;
	bool mObjectiveReached = false;


	UnitID mOwnerID;

	StateName mPrevState;

	State* mpCurrentState;

	State* mpHealthState;
	State* mpHuntState;
	State* mpBoostState;
	State* mpWanderState;

};
#endif // !STATEMACHINE_H
