#include "StateMachine.h"

StateMachine::StateMachine() : currentState(GETCHAR) {
	keywords["PROGRAMM"] = KEYWORDS;
}

StateMachine::~StateMachine() {}

void StateMachine::SetInput(std::string inp) {
	this->input = inp;
}

void StateMachine::SetState(int state) {
	this->currentState = state;
}

void StateMachine::Action() {
	int state = GETCHAR;
	
	// action on current state


	// set new state
	this->currentState = state;
}