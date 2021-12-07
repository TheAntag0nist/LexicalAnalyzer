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
#include "Token.h"

class LexicAnalyzer {
private:
	std::string data;
	std::list<Token> tokens;

public:
	LexicAnalyzer();
	~LexicAnalyzer();

	void ReadFile(std::string file);
	void ReadFile(std::ifstream file);

	void Analyze();
	void DisplayResults();

};

#endif  