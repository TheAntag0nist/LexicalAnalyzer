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
	bool isFunc;

	std::map<std::string, TOKENS_CLASS> dictionary;
	std::list<Token>* result;

	std::map<std::string, std::string> idTable;
	std::map<std::string, std::string> valTable;
	std::map<std::string, std::string> funcTable;

	static int uniqID;
	static int funcID;
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

	std::map<std::string, std::string>& GetMapId();
	std::map<std::string, std::string>& GetMapVal();
	std::map<std::string, std::string>& GetMapFunc();

};

#endif