#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "AnalyzeExceprions.h"
#include "StateMachine.h"
#include "Token.h"

using namespace std;

enum MESSAGE_TYPE {
	DEFAULT,
	LOST_DELIMITER,
	FUNC_ALREADY_EXIST
};

struct vertex {
	typedef pair<int, vertex*> ve;
	vector<ve> adj; //cost of edge, destination vertex
	string name;
	vertex(string s) : name(s) {}
};
class graph{
public:
	typedef map<string, vertex*> vmap;
	vmap work;
	void addvertex(const string&);
	void addedge(const string& from, const string& to, double cost = 0);
};

class Syntax {
private:
	// lexems
	std::list<Token>* lexems;
	std::list<Token>::iterator lexIter;
	// funcID, args
	std::map<std::string, std::list<std::string>> functions;
	std::string functionIdName;
	std::string errors;

	// helpers
	bool readPrototype;
	bool readArgs;
	// id's
	int funcUniqID;
	
	// main code
	graph tree;
	// functions tree
	std::list<graph> functionsTree;

	void S();

	void ID();
	void ARGS();
	void PROTOTYPE();

	void MAIN();

	void error(Token& currentToken, MESSAGE_TYPE messageType = DEFAULT);
	bool IsType(Token& currentToken);

public:
	void Action(StateMachine& machine, std::list<Token>* lexs);

};

#endif 
