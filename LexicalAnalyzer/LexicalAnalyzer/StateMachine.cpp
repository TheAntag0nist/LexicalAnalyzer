#include "StateMachine.h"

StateMachine::StateMachine() {}

StateMachine::~StateMachine() {}

void StateMachine::SetInput(std::string inp) {
	this->input = inp;
}

void StateMachine::SetState(int state) {
	this->currentState = state;
}

void StateMachine::Action() {
	// action on current state
}