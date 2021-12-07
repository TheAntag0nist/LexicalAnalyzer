#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <list>
#include <map>

#include "Token.h"

enum STATE {
	GETCHAR,
	DEFINETOKEN,
	END
};

class StateMachine {
private:
	std::string input;
	int currentState;

	std::map<std::string> keywords;
	std::map<std::string> operators;
	std::map<std::string> modifiers;
	std::map<std::string> delimiters;

	std::list<std::string> alphabet;

public:
	StateMachine();
	~StateMachine();

	void SetInput(std::string inp);
	void SetState(int state);
	void Action();

	void TryCheckToken();

};

#endif