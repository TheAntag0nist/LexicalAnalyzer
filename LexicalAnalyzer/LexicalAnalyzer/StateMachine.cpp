#include "StateMachine.h"

StateMachine::StateMachine() : currentState(GETCHAR), currentLine(1) {
	keywords["EXITWHILE"] = KEYWORDS;
	keywords["ENDWHILE"] = KEYWORDS;
	keywords["PROGRAMM"] = KEYWORDS;
	keywords["integer"] = KEYWORDS;
	keywords["string"] = KEYWORDS;
	keywords["WHILE"] = KEYWORDS;
	keywords["endif"] = KEYWORDS;
	keywords["else"] = KEYWORDS;
	keywords["FUNC"] = KEYWORDS;
	keywords["ENDF"] = KEYWORDS;
	keywords["bool"] = KEYWORDS;
	keywords["int"] = KEYWORDS;
	keywords["END"] = KEYWORDS;
	keywords["DO"] = KEYWORDS;
	keywords["if"] = KEYWORDS;

	modifiers["CONST"] = MODIFIER;
	modifiers["const"] = MODIFIER;
	
	delimiters["'"]  = DELIMITERS;
	delimiters["\""] = DELIMITERS;
	delimiters[" "]  = DELIMITERS;
	delimiters[";"]  = DELIMITERS;	
	delimiters[" "] = DELIMITERS;

	operators["+"] = OPERATORS;
	operators["-"] = OPERATORS;
	operators["/"] = OPERATORS;
	operators["*"] = OPERATORS;
	operators["="] = OPERATORS;
}

StateMachine::~StateMachine() {}

void StateMachine::SetResultList(std::list<Token>* list) {
	result = list;
}

void StateMachine::SetInput(std::string inp) {
	this->input = inp;
}

void StateMachine::SetState(int state) {
	this->currentState = state;
}

void StateMachine::Action() {
	std::string readData;
	int state = GETCHAR;
	
	// action on current state
	if (input == "")
		throw StateMachineException("sequence of input characters can't be null");

	for (int i = 0; i < input.length(); ++i) {
		if (input[i] == '\n')
			++currentLine;

		if (!IsChar(input[i]) && !IsDigit(input[i]) ||
			IsDelimiter(input[i]) || IsOperator(input[i]))
			currentState = DEFINETOKEN;

		if (currentState == GETCHAR &&
			(IsChar(input[i]) || IsDigit(input[i])))
			readData += input[i];

		if (currentState == DEFINETOKEN) {
#ifdef _DEBUG
			TryCheckToken(readData);
#elif NDEBUG 
			if (!TryCheckToken(readData))
				throw StateMachineException("Unknown lexic on line -> "
					+ std::to_string(currentLine) + " :: lexic -> " + readData);
#endif
			readData = "";
		}
	}

	// set new state
	this->currentState = END;
}

bool StateMachine::TryCheckToken(std::string tokenStr) {
	Token token;

	if (tokenStr == "") {
		currentState = GETCHAR;
		return true;
	}

	if (keywords.find(tokenStr) != keywords.end()) {
		// we found keyword
		token.SetName("keyword");
		token.SetCodeData(tokenStr);
		token.SetLine(currentLine);

		currentState = GETCHAR;

		result->push_back(token);
		return true;
	}
	if (operators.find(tokenStr) != operators.end()) {
		// we found operator		// we found keyword
		token.SetName("operator");
		token.SetCodeData(tokenStr);
		token.SetLine(currentLine);

		currentState = GETCHAR;

		result->push_back(token);
		return true;
	}
	if (delimiters.find(tokenStr) != delimiters.end()) {
		// we found delimiter
		token.SetName("delimiter");
		token.SetCodeData(tokenStr);
		token.SetLine(currentLine);

		currentState = GETCHAR;

		result->push_back(token);
		return true;
	}
	if (modifiers.find(tokenStr) != modifiers.end()) {
		// we found modifiers
		// only one variant in current version
		token.SetName("modifier");
		token.SetValue("CONST");
		token.SetCodeData(tokenStr);
		token.SetLine(currentLine);

		currentState = GETCHAR;

		result->push_back(token);
		return true;
	}
	if (!IsNumber(tokenStr)) {
		// we found indentifier
		token.SetName("ID");
		token.SetValue("test 42152");
		token.SetCodeData(tokenStr);
		token.SetLine(currentLine);

		currentState = GETCHAR;

		result->push_back(token);
		return true;
	}

	// meaybe we need more info
	currentState = GETCHAR;
	return false;
}

bool StateMachine::IsChar(char ch) {
	if ((ch >= 65 && ch <= 90) ||
		(ch >= 97 && ch <= 122))
		return true;

	return false;
}

bool StateMachine::IsDigit(char ch) {
	if (ch >= 48 && ch <= 57)
		return true;

	return false;
}

bool StateMachine::IsDelimiter(char ch) {
	std::string delim = "";
	delim = ch;

	if (delimiters.find(delim) != delimiters.end())
		return true;

	return false;
}

bool StateMachine::IsOperator(char ch) {
	std::string oper = "";
	oper = ch;

	if (operators.find(oper) != operators.end())
		return true;

	return false;
}

bool StateMachine::IsNumber(std::string data) {
	for (int i = 0; i < data.length(); ++i)
		if (!IsDigit(data[i]))
			return false;

	return true;
}
