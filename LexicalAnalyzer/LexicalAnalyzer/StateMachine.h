#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

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

public:
	StateMachine();
	~StateMachine();

	void SetInput(std::string inp);
	void SetState(int state);
	void Action();

};

#endif