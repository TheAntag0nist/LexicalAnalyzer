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

	std::map<std::string, TOKENS_CLASS> keywords;
	std::map<std::string, TOKENS_CLASS> operators;
	std::map<std::string, TOKENS_CLASS> modifiers;
	std::map<std::string, TOKENS_CLASS> delimiters;

	std::list<std::string> alphabet;
	std::list<Token>* result;

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

};

#endif