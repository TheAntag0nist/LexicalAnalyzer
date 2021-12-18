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
	cout << "\nVertex already exists!";
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
	lexems = lexs;
	S();
}

void Syntax::S() {
	lexIter = lexems->begin();
	// create root node for AST
	tree.addvertex("S");

	while (lexIter->GetName() == "keyword" &&
		lexIter->GetValue() == "FUNC")
		PROTOTYPE();
	
	PROGRAM();

	while (lexIter->GetName() == "keyword" &&
		lexIter->GetValue() == "FUNC")
	FUNC();
}

void Syntax::PROTOTYPE() {
	Token currentToken = *(++lexIter);
	bool funcID = false;
	bool type = false;
	char braceFlag = 0;

	tree.addvertex("PROTOTYPE");

	if (currentToken.GetName() == "ID") {
		funcID = true;
	}
	else
		throw SyntaxException("syntax:> func indentifier error: line -> " +
			std::to_string(currentToken.GetLine()) + " lexem -> " + currentToken.GetValue());

	currentToken = *(++lexIter);
	if (currentToken.GetName() == "delimiter" &&
		currentToken.GetValue() == "(")
		braceFlag++;
	else
		throw SyntaxException("syntax:> error: line -> " +
			std::to_string(currentToken.GetLine()) + " lexem -> " + currentToken.GetValue());

	tree.addvertex(currentToken.GetValue());
	tree.addedge("PROTOTYPE", currentToken.GetValue());

	++lexIter;
	while (lexIter != lexems->end()) {
		currentToken = *(lexIter);

		if (currentToken.GetName() == "delimiter" &&
			currentToken.GetValue() == ")")
			braceFlag++;

		if(currentToken.GetName() == "delimiter" &&
			currentToken.GetValue() == ";")
			break;

		if (currentToken.GetName() == "keyword" &&
			(currentToken.GetValue() == "string" ||
				currentToken.GetValue() == "integer" ||
				currentToken.GetValue() == "bool"))
			type = true;
		else if (currentToken.GetName() == "ID" && type) {
			type = false;
		}
		else if(braceFlag != 2)
			throw SyntaxException("syntax:> type error: line -> " +
			std::to_string(currentToken.GetLine()) + " lexem -> " + currentToken.GetValue());

		++lexIter;
	}

	if(braceFlag != 2)
		throw SyntaxException("syntax:> fatal error with braces");

	std::cout << "syntax_stage:> success -> read prototype;\n";
}

void Syntax::PROGRAM() {
	Token currentToken = *(++lexIter);

	if (currentToken.GetName() == "keyword" &&
		currentToken.GetValue() == "PROGRAMM") {
		currentToken = *(++lexIter);
		if ((currentToken.GetValue() == ";")) {
			std::cout << "syntax_stage:> success -> find main program;\n";
			RPOGRAM_BODY();
		}
		else
			throw SyntaxException("syntax:> type error: line -> " +
				std::to_string(currentToken.GetLine()) + " lexem -> " + currentToken.GetValue());
	}
	else
		throw SyntaxException("syntax:> type error: line -> " +
			std::to_string(currentToken.GetLine()) + " lexem -> " + currentToken.GetValue());

	tree.addvertex("PROGRAM");
	tree.addedge("S", "PROGRAM");
}

void Syntax::FUNC() {
	Token currentToken = *lexIter;

	// create func tree as a separate
	graph funcAST;

	// create AST

	functions.push_back(funcAST);
	++lexIter;
}

void Syntax::RPOGRAM_BODY(bool inner) {
	++lexIter;

	std::cout << "syntax_stage:> success -> start program body;\n";

	if (lexIter->GetValue() == "endif" ||
		lexIter->GetValue() == "ENDWHILE")
		return;

	// not nessacary
	// program can be without arguments
	while ((lexIter->GetName() == "keyword") &&
		(lexIter->GetValue() == "string" ||
			lexIter->GetValue() == "integer" ||
			lexIter->GetValue() == "bool") ||
		lexIter->GetName() == "ID") {
		IO();
		++lexIter;
	}

	std::cout << "syntax_stage:> success -> success all read IOs;\n";

	sequencesFlag = true;
	while(sequencesFlag)
		SEQUENCES();

	std::cout << "syntax_stage:> success -> read body;\n";

	// not necessary part
	if(inner)
		FUNC();
}

void Syntax::IO() {
	Token currentToken = *lexIter;
	Token tempToken = currentToken;

	if (currentToken.GetName() == "keyword") {
		if (currentToken.GetValue() == "string" ||
			currentToken.GetValue() == "integer" ||
			currentToken.GetValue() == "bool") {
			++lexIter;

			currentToken = *lexIter;
			if (currentToken.GetName() == "ID") {
				Token idTokenTemp = currentToken;
				
				++lexIter;
				currentToken = *lexIter;
				if (currentToken.GetName() == "delimiter" ||
					currentToken.GetValue() == ";") {
					// save to ast tree
				}
				else
					throw SyntaxException("syntax: error: need \';\' line -> " +
						std::to_string(currentToken.GetLine()) + " lexem -> " + currentToken.GetValue());
			}
			else
				throw SyntaxException("syntax: error: line -> " +
					std::to_string(currentToken.GetLine()) + " lexem -> " + currentToken.GetValue());
		}
	}
	else if (currentToken.GetName() == "ID") {
		++lexIter;

		// read next iter
		currentToken = *lexIter;
		if (currentToken.GetName() == "operator" && 
			currentToken.GetValue() == "=") {
			EXPRESSION(tempToken);
		}
		else
			throw SyntaxException("syntax:> error: line -> " +
				std::to_string(currentToken.GetLine()) + " lexem -> " + currentToken.GetValue());
	}else
		throw SyntaxException("syntax:> error: line -> " +
			std::to_string(currentToken.GetLine()) + " lexem -> " + currentToken.GetValue());

	std::cout << "syntax_stage:> success -> read IO;\n";
}

void Syntax::SEQUENCES() {
	Token currentToken = *lexIter;

	if (currentToken.GetValue() == "END") {
		sequencesFlag = false;
		return;
	}

	// start working on sequences
	CONSTRUCTION();

	++lexIter;
}

void Syntax::RETURN() {
	Token currentToken = *lexIter;

	// return data
	// just search END. token
	if (currentToken.GetName() == "keyword" ||
		currentToken.GetValue() == "END." || 
		currentToken.GetValue() == "ENDF" ||
		currentToken.GetValue() == "ENDWHILE" || 
		currentToken.GetValue() == "endif") {
		std::cout << "syntax_stage:> success -> find END.;\n";
	}

	++lexIter;
}

void Syntax::EXPRESSION(Token leftId) {
	std::list<Token> expression;
	std::string leftIdType = leftId.GetType();
	bool boolExpression = false;

	++lexIter;
	while ((lexIter->GetName() != "delimiter" && 
		lexIter->GetName() != "keyword") &&
		lexIter != lexems->end()) {
		expression.push_back(*lexIter);

		if (lexIter->GetName() == "operator" ||
			lexIter->GetValue() == ">" ||
			lexIter->GetValue() == "<" ||
			lexIter->GetValue() == ">=" ||
			lexIter->GetValue() == "<=" ||
			lexIter->GetValue() == "==" ||
			lexIter->GetValue() == "!=")
			boolExpression = true;

		++lexIter;
	}

	bool ignoreFlag = boolExpression ? false : true;;
	if (leftIdType == "bool") {
		ignoreFlag = true;
	}

	if (lexIter == lexems->end())
		throw SyntaxException("syntax:> error: not find delimiter;");


	bool first = true;
	bool idOperatorFlag = true;
	for (auto it = expression.begin(); it != expression.end(); ++it) {
		if (first) {
			first = false;
			if (it->GetName() == "operator")
				idOperatorFlag = false;
		}

		if (it->GetValue() == "(" ||
			it->GetValue() == ")") {
			++lexIter;
			continue;
		}
		
		if (idOperatorFlag) {
			idOperatorFlag = false;
			if (it->GetName() == "ID" || it->GetName() == "CONSTVAL") {
				if (it->GetType() != leftIdType && ((leftIdType != "bool" &&
					it->GetType() == "integer") || (leftIdType != "integer" &&
						it->GetType() == "bool")))
					throw SyntaxException("syntax:> error invalid types: line -> " +
						std::to_string(it->GetLine()) + " lexem -> " + it->GetValue() +
						" left id type -> " + leftIdType +
						" right type -> " + (it->GetType() == "" ? "unknown" : it->GetType()));
				// add to ast tree
			}			
			else
				throw SyntaxException("syntax:> error: line -> " +
					std::to_string(it->GetLine()) + " lexem -> " + it->GetValue());

		}
		else if (!idOperatorFlag) {
			idOperatorFlag = true;
			if(it->GetName() == "operator" || it->GetName() == "keyword") {
				// convert to ast
				tree.addvertex(it->GetValue());
			}
			else
				throw SyntaxException("syntax:> need operator: line -> " +
					std::to_string(it->GetLine()) + " lexem -> " + it->GetValue());
		}
		else
			throw SyntaxException("syntax:> error: line -> " +
				std::to_string(it->GetLine()) + " lexem -> " + it->GetValue());
	}

	std::cout << "syntax_stage:> success -> read definition IO;\n";
}

void Syntax::CONSTRUCTION() {
	Token currentToken = *lexIter;

	if (currentToken.GetName() == "keyword") {
		if (currentToken.GetValue() == "WHILE")
			WHILE();
		if (currentToken.GetValue() == "if")
			IF();
	}
}

void Syntax::IF() {
	CONDITION();

	RPOGRAM_BODY(true);
}

void Syntax::WHILE() {
	whileFlag = true;

	std::cout << "syntax_stage:> success -> read WHILE;\n";

	tempGlobalToken.SetName("ID");
	tempGlobalToken.SetValue("temp");
	tempGlobalToken.SetType("bool");

	CONDITION();

	std::cout << "syntax_stage:> success -> read CONDITION;\n";

	if (lexIter->GetName() == "keyword" &&
		lexIter->GetValue() == "DO")
		RPOGRAM_BODY(true);
	else
		throw SyntaxException("syntax:> error: line -> " +
			std::to_string(lexIter->GetLine()) + " lexem -> " + lexIter->GetValue());
}

// it's just expression with bool type
void Syntax::CONDITION() {
	EXPRESSION(tempGlobalToken);
}