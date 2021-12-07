#include "StateMachine.h"

StateMachine::StateMachine() : currentState(GETCHAR) {
	keywords["PROGRAMM"] = KEYWORDS;

    keywords["CONST"] = KEYWORDS;
    keywords["const"] = KEYWORDS;
    keywords["'"]     = KEYWORDS;
    keywords["\""]    = KEYWORDS;
    keywords[" "]     = KEYWORDS;

	keywords["integer"] = KEYWORDS;
	keywords["bool"] = KEYWORDS;
	keywords["string"] = KEYWORDS;
	keywords["WHILE"] = KEYWORDS;
	keywords["DO"] = KEYWORDS;
	keywords["EXITWHILE"] = KEYWORDS;
	keywords["if"] = KEYWORDS;
	keywords["ENDWHILE"] = KEYWORDS;
	keywords["else"] = KEYWORDS;
	keywords["endif"] = KEYWORDS;
	keywords["FUNC"] = KEYWORDS;
	keywords["ENDF"] = KEYWORDS;
	keywords["END"] = KEYWORDS;
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
