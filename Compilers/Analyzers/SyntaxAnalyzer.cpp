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
}

void Syntax::PROGRAM() {
	Token currentToken = *(++lexIter);

	if (currentToken.GetName() == "keyword" &&
		currentToken.GetValue() == "PROGRAMM") {
		currentToken = *(++lexIter);
		if ((currentToken.GetValue() == ";")) {
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

	++lexIter;
}

void Syntax::RPOGRAM_BODY() {
	++lexIter;

	// not nessacary
	// program can be without arguments
	while(lexIter->GetName() == "keyword" && 
		(lexIter->GetValue() == "string" ||
		 lexIter->GetValue() == "integer" ||
		 lexIter->GetValue() == "bool" || 
		 lexIter->GetName() == "ID"))
		IO();

	SEQUENCES();
	RETURN();
}

void Syntax::IO() {
	Token currentToken = *lexIter;

	++lexIter;
}

void Syntax::SEQUENCES() {
	Token currentToken = *lexIter;

	++lexIter;
}

void Syntax::RETURN() {
	Token currentToken = *lexIter;

	++lexIter;
}