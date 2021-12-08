#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <list>
#include <map>

#include "AnalyzeExceprions.h"
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
	int currentLine;

	bool isString;

	std::map<std::string, TOKENS_CLASS> dictionary;

	std::list<std::string> alphabet;
	std::list<Token>* result; 

	static int uniqID;
	static int constNumID;

public:
	StateMachine();
	~StateMachine();

	void SetResultList(std::list<Token>* list);
	void SetInput(std::string inp);
	void SetState(int state);

	void Action();
	bool TryCheckToken(std::string tokenStr);

	bool IsChar(char ch);
	bool IsDigit(char ch);
	bool IsDelimiter(char ch);
	bool IsOperator(char ch);

	bool IsNumber(std::string data);
	bool IsId(std::string data);

};

#endif