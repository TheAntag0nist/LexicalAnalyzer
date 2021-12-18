#include "SyntaxAnalyzer.h"

void Syntax::Action(StateMachine& machine, 
	std::list<Token>* lexs) {
	lexems = lexs;
	S();
}

void Syntax::S() {
	lexIter = lexems->begin();
	// create root node for AST

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
}

void Syntax::FUNC() {
	Token currentToken = *lexIter;

	// create func tree as a separate
	AST funcAST;

	// create AST

	++lexIter;
}

void Syntax::RPOGRAM_BODY() {
	++lexIter;

	std::cout << "syntax_stage:> success -> start program body;\n";

	// not nessacary
	// program can be without arguments
	while((lexIter->GetName() == "keyword") &&
		(lexIter->GetValue() == "string" ||
		 lexIter->GetValue() == "integer" ||
		 lexIter->GetValue() == "bool") ||
		lexIter->GetName() == "ID")
		IO();

	std::cout << "syntax_stage:> success -> success all read IOs;\n";

	SEQUENCES();

	std::cout << "syntax_stage:> success -> read body;\n";

	// not necessary part
	RETURN();

	// not necessary part
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
	++lexIter;
}

void Syntax::SEQUENCES() {
	Token currentToken = *lexIter;

	// start working on sequences


	++lexIter;
}

void Syntax::RETURN() {
	Token currentToken = *lexIter;

	// return data
	// just search END. token

	++lexIter;
}

void Syntax::EXPRESSION(Token leftId) {
	std::list<Token> expression;
	std::string leftIdType = leftId.GetType();

	while (lexIter->GetName() != "delimiter" &&
		lexIter != lexems->end()) {
		expression.push_back(*lexIter);

		++lexIter;
	}

	if (lexIter == lexems->end())
		throw SyntaxException("syntax:> error: not find delimiter;");

	bool idOperatorFlag = true;
	for (auto it = ++expression.begin(); it != expression.end(); ++it) {
		if (idOperatorFlag) {
			idOperatorFlag = false;
			if (it->GetName() == "ID" || it->GetName() == "CONSTVAL") {
				if (it->GetType() != leftIdType)
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
			if(it->GetName() == "operator"){
				// convert to ast
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
	++lexIter;
	Token currentToken = *lexIter;

	if (currentToken.GetName() == "keyword") {
		if (currentToken.GetValue() == "WHILE")
			WHILE();
		if (currentToken.GetValue() == "if")
			IF();
	}
}