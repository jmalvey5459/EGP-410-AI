#ifndef STATE_H
#define STATE_H
#include "Transition.h"
class Transition;
class State :public Trackable
{

public:
	
	State();
	State(StateName name);
	~State();

	StateName getName();

	//adds transition to vector of transitions
	void addTransition(Transition in);

	//returns transition that is triggered first
	Transition checkTransitionTriggers();

	void operator=(const State& rState);



private:

	StateName mName;
	std::vector<Transition>* mpTransitions;
	
};
#endif // !STATE_H