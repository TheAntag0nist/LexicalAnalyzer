#include "SyntaxAnalyzer.h"

void graph::addvertex(const string& name)
{
	vmap::iterator itr = work.find(name);
	if (itr == work.end())
	{
		vertex* v;
		v = new vertex(name);
		work[name] = v;
		return;
	}
}
void graph::addedge(const string& from, const string& to, double cost)
{
	vertex* f = (work.find(from)->second);
	vertex* t = (work.find(to)->second);
	pair<int, vertex*> edge = make_pair(cost, t);
	f->adj.push_back(edge);
}

void Syntax::Action(StateMachine& machine, 
	std::list<Token>* lexs) {
	errors = "";
	lexems = lexs;
	lexIter = lexems->begin();
	funcUniqID = 0;

	// start analyze and create syntax tree
	tree.addvertex("S");
	S();

	// display all errors
	if (errors != "")
		throw SyntaxException(errors);
}

void Syntax::S() {
	PROTOTYPE();

	MAIN();
}

void Syntax::ID() {
	Token currentToken = *lexIter;

	if (currentToken.GetName() == "ID") {
		if (readPrototype && !readArgs) {
			std::string tempName = "FUNC" + std::to_string(funcUniqID);
			tree.addvertex(currentToken.GetCodeData());
			tree.addedge(tempName, currentToken.GetCodeData());

			// save function name in map
			functionIdName = currentToken.GetCodeData();
			if (functions.find(functionIdName) == functions.end())
				functions[functionIdName];
			else
				error(currentToken, FUNC_ALREADY_EXIST);
		}
	}else
		error(currentToken);
}

void Syntax::ARGS() {
	Token currentToken = *lexIter;

	if (currentToken.GetName() == "keyword" &&
		IsType(currentToken)) {
		// read type
		if (readPrototype && readArgs) {
			std::list<std::string>& args = functions[functionIdName];
			args.push_back(currentToken.GetValue());
		}

		++lexIter;
		ID();
	}
	else if(currentToken.GetValue() != ")")
		error(currentToken);

	++lexIter;
	currentToken = *lexIter;
	if (currentToken.GetValue() == ",") {
		++lexIter;
		ARGS();
	}
}

void Syntax::PROTOTYPE() {
	Token currentToken = *lexIter;

	tree.addvertex("PROTOTYPE");
	tree.addedge("S", "PROTOTYPE");

	readPrototype = true;

	if (currentToken.GetValue() == "FUNC") {
		std::string tempName = "FUNC" + std::to_string(++funcUniqID);
		tree.addvertex(tempName);
		tree.addedge("PROTOTYPE", tempName);

		++lexIter;
		ID();
		++lexIter;
		currentToken = *lexIter;

		if (currentToken.GetName() == "delimiter" &&
			currentToken.GetValue() == "(") {
			// read args
			// for correct reading ID's
			readArgs = true;
			++lexIter;
			ARGS();
		}
		else
			error(currentToken);
	}
	else if (currentToken.GetValue() != "PROGRAM")
		error(currentToken);

	readPrototype = false;
	readArgs = false;

	++lexIter;
	if (lexIter->GetValue() != ";")
		error(currentToken, LOST_DELIMITER);
	++lexIter;
	if (lexIter->GetValue() == "FUNC")
		PROTOTYPE();
}


void Syntax::error(Token& currentToken, MESSAGE_TYPE messageType) {
	switch (messageType){
	case DEFAULT:
		errors += "syntax:> line:> " + std::to_string(currentToken.GetLine()) +
			" in " + currentToken.GetValue() + "\n";
		break;
	case FUNC_ALREADY_EXIST:
		errors += "syntax:> line:> " + std::to_string(currentToken.GetLine()) +
			" in " + currentToken.GetCodeData() + " -> func alredy exist\n";
		break;
	case LOST_DELIMITER:
		errors += "syntax:> line:> " + std::to_string(currentToken.GetLine()) +
			" in " + currentToken.GetValue() + " -> lost delimiter\n";
		break;
	}
}

bool Syntax::IsType(Token& currentToken) {
	if (currentToken.GetValue() == "bool" ||
		currentToken.GetValue() == "string" ||
		currentToken.GetValue() == "integer")
		return true;

	return false;
}