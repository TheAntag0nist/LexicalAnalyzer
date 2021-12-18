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

struct vertex {
	typedef pair<int, vertex*> ve;
	vector<ve> adj; //cost of edge, destination vertex
	string name;
	vertex(string s) : name(s) {}
};
class graph
{
public:
	typedef map<string, vertex*> vmap;
	vmap work;
	void addvertex(const string&);
	void addedge(const string& from, const string& to, double cost = 0);
};

class Syntax {
private:
	std::list<Token>* lexems;
	std::list<Token>::iterator lexIter;
	
	Token tempGlobalToken;
	
	bool sequencesFlag;
	bool whileFlag;
	bool ifFlag;
	
	// main code
	graph tree;
	// functions
	std::list<graph> functions;

	void S();

	void PROTOTYPE();
	void PROGRAM();
	void FUNC();

	void RPOGRAM_BODY(bool inner = false);

	void IO();
	void SEQUENCES();
	void RETURN();

	void EXPRESSION(Token leftId);
	void CONSTRUCTION();

	void IF();
	void WHILE();

	void CONDITION();

public:
	void Action(StateMachine& machine, std::list<Token>* lexs);

};

#endif 
