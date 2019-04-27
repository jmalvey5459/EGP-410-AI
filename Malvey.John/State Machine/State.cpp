#include "State.h"
State::State()
{
	mName = INVALID_STATE;
	mpTransitions = new std::vector<Transition>;
}
State::State(StateName name)
{
	mName = name;
//	if (name == SEEK_BOOST || name == SEEK_HEALTH)
	//	mHasObjective = true;
	mpTransitions = new std::vector<Transition>;
};
State::~State()
{

}
Transition State::checkTransitionTriggers()
{
	Transition temp;
	for (unsigned int i = 0; i < mpTransitions->size(); i++)
	{
		if (mpTransitions->at(i).isTriggered())
			return (mpTransitions->at(i));
		}
	return temp;
}

StateName State::getName()
{
	return mName;
}

void State::addTransition(Transition in)
{
	mpTransitions->push_back(in); 
}

void State::operator=(const State& rState)
{
	this->mName = rState.mName;
	this->mpTransitions = rState.mpTransitions;
}
