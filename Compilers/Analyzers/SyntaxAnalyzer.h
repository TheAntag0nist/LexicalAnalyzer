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
private:
	Node root;

public:
	AST() {}
	~AST() {}

};

class Syntax {
private:
	std::list<Token>* lexems;
	std::list<Token>::iterator lexIter;
	AST tree;

	void S();

	void PROTOTYPE();
	void PROGRAM();
	void FUNC();

	void RPOGRAM_BODY();

	void IO();
	void SEQUENCES();
	void RETURN();

public:
	void Action(StateMachine& machine, std::list<Token>* lexs);

};

#endif 
