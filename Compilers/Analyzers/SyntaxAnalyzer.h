#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include <vector>
#include <list>

#include "AnalyzeExceprions.h"
#include "StateMachine.h"
#include "Token.h"

typedef struct Node{
	Node* parent;
	Token data;

	std::vector<Node> branches;

	Node() {
		parent = nullptr;
	}
} Node;

class AST {
public:
	Node root;

	AST() {}
	~AST() {}

};

class Syntax {
private:
	std::list<Token>* lexems;
	std::list<Token>::iterator lexIter;
	
	// main code
	AST tree;
	// functions
	std::list<AST> functions;

	void S();

	void PROTOTYPE();
	void PROGRAM();
	void FUNC();

	void RPOGRAM_BODY();

	void IO();
	void SEQUENCES();
	void RETURN();

	void EXPRESSION(Token leftId);
	void CONSTRUCTION();
	
	void CONDITION();

	void IF();
	void WHILE();

public:
	void Action(StateMachine& machine, std::list<Token>* lexs);

};

#endif 
