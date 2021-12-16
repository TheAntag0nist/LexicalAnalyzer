#ifndef LEXIC_ANALYZER_H
#define LEXIC_ANALYZER_H

#ifdef _DEBUG
#include <iostream>
#include <iomanip>
#endif

#include <sstream>
#include <fstream>
#include <string>
#include <list>

#include "AnalyzeExceprions.h"
#include "StateMachine.h"
#include "Token.h"

class Parser {
private:
	std::string data;
	std::list<Token> tokens;

	std::list<std::string> indentifier;
	std::list<std::string> constants;

	StateMachine machine;

public:
	Parser();
	~Parser();

	void ReadFile(std::string file);
	void ReadFile(std::ifstream file);

	void Analyze();
	void DisplayResults();

};

#endif
