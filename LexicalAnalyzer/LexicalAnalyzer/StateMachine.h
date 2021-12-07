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
	int currentState;

public:


};

#endif