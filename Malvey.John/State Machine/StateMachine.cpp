#pragma once
#include "StateMachine.h"
StateMachine::StateMachine()
{
	mpHealthState = new State(INVALID_STATE);
	mpHuntState = new State(INVALID_STATE);
	mpBoostState = new State(INVALID_STATE);
	mpWanderState = new State(INVALID_STATE);
}
StateMachine::StateMachine(UnitID& owner)
{	//set unit affected by this state machine

	mOwnerID = owner;
	//mpInitialState = new State(INVALID_STATE);
	mpCurrentState = new State(INVALID_STATE);

	mpHealthState = new State(SEEK_HEALTH);
	mpHuntState = new State(HUNT_PLAYER);
	mpBoostState = new State(SEEK_BOOST);
	mpWanderState = new State(WANDER);
	init();
}
void StateMachine::init()
{
	///set up transitions
	//transitions from wander state
	Transition WanderToHuntPlayer(mOwnerID, IF_PLAYER_IN_RANGE, HUNT_PLAYER);
	Transition WanderToSeekBoost(mOwnerID, IF_BOOST_IN_RANGE, SEEK_BOOST);

	//transitions from player hunting state
	Transition HuntPlayerToSeekHealth(mOwnerID, IF_LOW_HEALTH, SEEK_HEALTH);
	Transition HuntPlayerToWander(mOwnerID, IF_NOTHING_IN_RANGE, WANDER);
	Transition HuntPlayerToSeekBoost(mOwnerID, IF_BOOST_CLOSER_THAN_PLAYER, SEEK_BOOST);

	//transitions from boost seeking state
	Transition SeekBoostToWander(mOwnerID, IF_NOTHING_IN_RANGE, WANDER);
	Transition SeekBoostToHuntPlayer(mOwnerID, IF_PLAYER_IN_RANGE, HUNT_PLAYER);

	//transitions from health seeking state
	Transition SeekHealthToHuntPlayer(mOwnerID, IF_PLAYER_IN_RANGE, HUNT_PLAYER);
	Transition SeekHealthToSeekBoost(mOwnerID, IF_BOOST_CLOSER_THAN_PLAYER, SEEK_BOOST);


	//initialize states

	//health
	mpHealthState->addTransition(SeekBoostToHuntPlayer);
	mpHealthState->addTransition(SeekHealthToSeekBoost);

	//hunt
	mpHuntState->addTransition(HuntPlayerToSeekBoost);
	mpHuntState->addTransition(HuntPlayerToSeekHealth);
	mpHuntState->addTransition(HuntPlayerToWander);

	//boost
	mpBoostState->addTransition(SeekBoostToHuntPlayer);
	mpBoostState->addTransition(SeekBoostToWander);

	//wander
	mpWanderState->addTransition(WanderToHuntPlayer);
	mpWanderState->addTransition(WanderToSeekBoost);

	//initialize tracking(?) states
	delete mpCurrentState;
	mpCurrentState = NULL;
	mpCurrentState = mpWanderState;

	

	
}
StateMachine::~StateMachine()
{
	
	if (mpHealthState != NULL)
	{
		delete mpHealthState;
		mpHealthState = NULL;
	}

	if (mpHuntState != NULL)
	{
		delete mpHuntState;
		mpHuntState = NULL;
	}

	if (mpBoostState != NULL)
	{
		delete mpBoostState;
		mpBoostState = NULL;
	}

	if (mpWanderState != NULL)
	{
		delete mpWanderState;
		mpWanderState = NULL;
	}

}


State* StateMachine::getCurrentState()
{
	return mpCurrentState;
}

void StateMachine::update()
{
	mPrevState = mpCurrentState->getName();
	//if the state is objective, don't allow switiching until objective is complete.
	//otherwise, go for it
	if(!mCurrentStateIsAnObjective || (mCurrentStateIsAnObjective && mObjectiveReached))
	{
		mCurrentStateIsAnObjective = false;
		mObjectiveReached = false;
	
		switch (mpCurrentState->checkTransitionTriggers().getTargetState()) {
		case HUNT_PLAYER:
			//delete mpCurrentState;
			//mpCurrentState = NULL;
			mpCurrentState = mpHuntState;
			break;
		case SEEK_BOOST:
			//delete mpCurrentState;
			//mpCurrentState = NULL;
			mpCurrentState = mpBoostState;
			break;
		case SEEK_HEALTH:
			//delete mpCurrentState;
			//mpCurrentState = NULL;
			mpCurrentState = mpHealthState;
			break;
		case WANDER:
			//delete mpCurrentState;
			//mpCurrentState = NULL;
			mpCurrentState = mpWanderState;
			break;
		}
	}
	if (mpCurrentState->getName() == SEEK_BOOST || mpCurrentState->getName() == SEEK_HEALTH)
	{
		mObjectiveReached = false;
		mCurrentStateIsAnObjective = true;
	}
}
StateName StateMachine::getPrevStateName()
{
	return mPrevState;
}
void StateMachine::deleteAllStates()
{
	if (mpBoostState != NULL)
	{
		delete mpBoostState;
		mpBoostState = NULL;
	}
	if (mpHuntState != NULL)
	{
		delete mpHuntState;
		mpHuntState = NULL;
	}
	if (mpHealthState != NULL)
	{
		delete mpHealthState;
		mpHealthState = NULL;
	}
	if (mpWanderState != NULL)
	{
		delete mpWanderState;
		mpWanderState = NULL;
	}
}
void StateMachine::objectiveSuccess()
{
	mObjectiveReached = true;
}