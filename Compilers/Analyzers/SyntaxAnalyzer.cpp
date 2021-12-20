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
	uniq = 0;

	// start analyze and create syntax tree
	tree.addvertex("S");
	S();

	// display all errors
	if (errors != "")
		throw SyntaxException(errors);
	else
		std::cout << "syntax:> success" << std::endl;
}

void Syntax::S() {
	// read prototypes
	PROTOTYPE();

	MAIN();
	afterMainProg = true;

	FUNC();
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
		else if(readIO){
			// save id
			std::string tempName = currentToken.GetCodeData() + " " +
				std::to_string(++uniq);
			
			tree.addvertex(tempName);
			tree.addedge("IO", tempName);
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
		error(currentToken, INCORRECT_TYPE);

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

void Syntax::MAIN() {
	Token currentToken = *lexIter;

	tree.addvertex("MAIN");
	tree.addedge("S", "MAIN");

	// check MAIN PROGRAM
	if (currentToken.GetValue() == "PROGRAMM") {
		tree.addvertex("PROGRAMM");
		tree.addedge("S", "PROGRAMM");
	}
	else
		error(currentToken, EXTRA, "not find PROGRAM");

	++lexIter;
	currentToken = *lexIter;

	if (currentToken.GetValue() != ";")
		error(currentToken, LOST_DELIMITER);


	tree.addvertex("BODY");
	tree.addedge("MAIN", "BODY");

	++lexIter;
	BODY();

	// check end body
	++lexIter;
	currentToken = *lexIter;

	if (currentToken.GetValue() == "END") {
		++lexIter;
		currentToken = *lexIter;

		if (currentToken.GetValue() == ".") {
			tree.addvertex("END.");
			tree.addedge("S", "END.");
		}
		else
			error(currentToken, LOST_DELIMITER);
	}
	else
		error(currentToken);
}

// big minus - recursion
void Syntax::BODY() {
	Token currentToken = *lexIter;

	if (currentToken.GetName() == "keyword") {
		IO();
		IF();
		WHILE();
	}
	if (currentToken.GetName() == "ID") {
		prevToken = currentToken;
		++lexIter;
		EXPRESSION();
	}

	auto tempIter = lexIter;
	++tempIter;
	if (tempIter->GetValue() == "END") {
		++tempIter;
		if (tempIter->GetValue() == ".")
			return;
	}
	else {
		++lexIter;
		BODY();
	}
}

void Syntax::IO() {
	Token currentToken = *lexIter;

	tree.addvertex("IO");
	tree.addedge("BODY", "IO");

	readIO = true;

	// IO part
	if (IsType(currentToken)) {
		std::string tempType = currentToken.GetValue();

		// save type
		tree.addvertex(tempType);
		tree.addedge("IO", tempType);

		++lexIter;
		ID();

		if (tempType != lexIter->GetType())
			error(currentToken, EXTRA, "incorrect type -> " + tempType);
		++lexIter;
		if (lexIter->GetValue() != ";")
			error(currentToken, LOST_DELIMITER);
	}

	readIO = false;
}

void Syntax::IF() {
	Token currentToken = *lexIter;

	if (currentToken.GetValue() == "if")
		while (lexIter->GetValue() != "endif")
			++lexIter;
}

void Syntax::WHILE() {
	Token currentToken = *lexIter;

	if(currentToken.GetValue() == "WHILE")
		while (lexIter->GetValue() != "ENDWHILE")
			++lexIter;
}

void Syntax::EXPRESSION() {
	Token currentToken = *lexIter;
	std::stack<Token> polkStack;
	std::vector<Token> polkNotation;
	std::string tempType = prevToken.GetType();

	// save type
	std::string tempName = "EXPRESSION " + std::to_string(++uniq);
	tree.addvertex(tempName);
	tree.addedge("BODY", tempName);

	std::string id = prevToken.GetCodeData() + " " + std::to_string(++uniq);
	tree.addvertex(id);
	std::string operatorSymbol = "= " + std::to_string(++uniq);
	tree.addvertex(operatorSymbol);

	tree.addedge(operatorSymbol, id);
	tree.addedge(tempName, operatorSymbol);

	// polk notation for tree
	if (currentToken.GetName() == "operator" &&
		(currentToken.GetValue() == "=" ||
			currentToken.GetValue() == "==" ||
			currentToken.GetValue() == ">" ||
			currentToken.GetValue() == ">=" ||
			currentToken.GetValue() == "<" ||
			currentToken.GetValue() == "<=")) {
		// start read
		while(lexIter->GetValue() != ";") {
			currentToken = *lexIter;
			// if function or id
			if (lexIter->GetName() == "ID") {
				if (functions.find(lexIter->GetCodeData()) != functions.end()) {
					polkNotation.push_back(*lexIter);

					// not check functions
					// need rewrite in future
					while (lexIter->GetValue() != ")")
						++lexIter;
				}
				else {
					if (lexIter->GetType() != tempType)
						error(currentToken, INCORRECT_TYPE);
					polkNotation.push_back(*lexIter);
				}
			}

			// if const val
			if (lexIter->GetName() == "CONSTVAL") {
				if (lexIter->GetType() != tempType)
					error(currentToken, INCORRECT_TYPE);
				polkNotation.push_back(*lexIter);
			}

			// if operator
			if (lexIter->GetName() == "operator" &&
				(lexIter->GetValue() == "+" || lexIter->GetValue() == "-")) {

				if(!polkStack.empty())
					while (!polkStack.empty()) {
						if (polkStack.top().GetValue() == "*" ||
							polkStack.top().GetValue() == "/" ||
							polkStack.top().GetValue() == "+" ||
							polkStack.top().GetValue() == "-") {
							polkNotation.push_back(polkStack.top());
							polkStack.pop();
						}
						else
							break;
					}

				polkStack.push(*lexIter);
			}

			if (lexIter->GetName() == "operator" &&
				(lexIter->GetValue() == "*" || lexIter->GetValue() == "/")) {
				if (!polkStack.empty())
					while (!polkStack.empty()) {
						if (polkStack.top().GetValue() == "*" ||
							polkStack.top().GetValue() == "/") {
							polkNotation.push_back(polkStack.top());
							polkStack.pop();
						}
						else
							break;
					}

				polkStack.push(*lexIter);
			}

			// id (
			if(lexIter->GetValue() == "(")
				polkStack.push(*lexIter);

			// if )
			if (lexIter->GetValue() == ")") {
				if (!polkStack.empty())
					while (polkStack.top().GetValue() != "(") {
						polkNotation.push_back(polkStack.top());
						polkStack.pop();
					}

				if (!polkStack.empty())
					polkStack.pop();
			}

			++lexIter;
		}
	}
	else
		error(currentToken, EXTRA, "bad expression");

	if (!polkStack.empty())
		while (!polkStack.empty()) {
			polkNotation.push_back(polkStack.top());
			polkStack.pop();
		}

	// start create tree from polk stack
	// only binary operations
	// read from end
	// for creating tree
	if (polkNotation.size() > 2) {
		int currentOperatorOffset = 0;
		std::list<std::string> operatorTokens;

		for (auto it = --polkNotation.end(); it != polkNotation.begin(); --it) {
			// link operators
			if (it->GetName() == "operator") {
				std::string tempName = it->GetValue() + " " + std::to_string(++uniq);
				tree.addvertex(tempName);
				if (!operatorTokens.empty()) {
					auto last = --operatorTokens.end();
					tree.addedge(*last, tempName);
				}
				operatorTokens.push_back(tempName);
			}

			// if id or const val
			if (it->GetName() == "CONSTVAL" || it->GetName() == "ID") {
				std::list<std::string>::iterator lastWithOffset = --operatorTokens.end();
				for (int i = 0; i < currentOperatorOffset; ++i)
					--lastWithOffset;

				if (tree.work[*lastWithOffset]->adj.size() == 2)
					++currentOperatorOffset;

				// recalculate
				lastWithOffset = --operatorTokens.end();
				for (int i = 0; i < currentOperatorOffset; ++i)
					--lastWithOffset;

				std::string tempName = it->GetCodeData() + " " + std::to_string(++uniq);
				tree.addvertex(tempName);
				tree.addedge(*lastWithOffset, tempName);
			}
		}

		auto it = polkNotation.begin();

		// link operators
		if (it->GetName() == "operator") {
			std::string tempName = it->GetValue() + " " + std::to_string(++uniq);
			tree.addvertex(tempName);
			if (!operatorTokens.empty()) {
				auto last = --operatorTokens.end();
				tree.addedge(*last, tempName);
			}
			operatorTokens.push_back(tempName);
		}

		// if id or const val
		if (it->GetName() == "CONSTVAL" || it->GetName() == "ID") {
			std::list<std::string>::iterator lastWithOffset = --operatorTokens.end();
			for (int i = 0; i < currentOperatorOffset; ++i)
				--lastWithOffset;

			if (tree.work[*lastWithOffset]->adj.size() == 2)
				++currentOperatorOffset;

			// recalculate
			lastWithOffset = --operatorTokens.end();
			for (int i = 0; i < currentOperatorOffset; ++i)
				--lastWithOffset;

			std::string tempName = it->GetCodeData() + " " + std::to_string(++uniq);
			tree.addvertex(tempName);
			tree.addedge(*lastWithOffset, tempName);
		}

		std::list<std::string>::iterator lastWithOffset = operatorTokens.begin();

		tree.addedge(operatorSymbol, *lastWithOffset);
	}
	else {
		auto it = polkNotation.begin();

	}
}

void Syntax::FUNC() {

}


void Syntax::error(Token& currentToken, MESSAGE_TYPE messageType, std::string description) {
	switch (messageType){
	case DEFAULT:
		errors += "syntax:> line:> " + std::to_string(currentToken.GetLine()) +
			" in " + currentToken.GetValue() + ":" + currentToken.GetCodeData() +
			"\n";
		break;
	case FUNC_ALREADY_EXIST:
		errors += "syntax:> line:> " + std::to_string(currentToken.GetLine()) +
			" in " + currentToken.GetValue() + ":" + currentToken.GetCodeData() +
			" -> func alredy exist\n";
		break;
	case LOST_DELIMITER:
		errors += "syntax:> line:> " + std::to_string(currentToken.GetLine()) +
			" in " + currentToken.GetValue() + ":" + currentToken.GetCodeData() +
			" -> lost delimiter\n";
		break;
	case INCORRECT_TYPE:
		errors += "syntax:> line:> " + std::to_string(currentToken.GetLine()) +
			" in " + currentToken.GetValue() + ":" + currentToken.GetCodeData() +
			" ->  incorrect type\n";
		break;
	case EXTRA:
		errors += "syntax:> line:> " + std::to_string(currentToken.GetLine()) +
			" in " + currentToken.GetValue() + ":" + currentToken.GetCodeData() +
			" -> " + description + "\n";
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