#include "StateMachine.h"

int StateMachine::uniqID = 0;
int StateMachine::funcID = 0;
int StateMachine::constNumID = 0;

StateMachine::StateMachine() : currentState(GETCHAR), currentLine(1) {
	isString = false;
	isFunc = false;

	dictionary["EXITWHILE"] = KEYWORDS;
	dictionary["ENDWHILE"] = KEYWORDS;
	dictionary["PROGRAMM"] = KEYWORDS;
	dictionary["integer"] = KEYWORDS;
	dictionary["string"] = KEYWORDS;
	dictionary["WHILE"] = KEYWORDS;
	dictionary["endif"] = KEYWORDS;
	dictionary["else"] = KEYWORDS;
	dictionary["FUNC"] = KEYWORDS;
	dictionary["ENDF"] = KEYWORDS;
	dictionary["then"] = KEYWORDS;
	dictionary["bool"] = KEYWORDS;
	dictionary["int"] = KEYWORDS;
	dictionary["END"] = KEYWORDS;
	dictionary["DO"] = KEYWORDS;
	dictionary["if"] = KEYWORDS;

	dictionary["CONST"] = MODIFIER;
	dictionary["const"] = MODIFIER;
	
	dictionary["'"]  = DELIMITERS;
	dictionary[";"]  = DELIMITERS;	
	dictionary[" "] = DELIMITERS;
	dictionary["("] = DELIMITERS;
	dictionary[")"] = DELIMITERS;

	dictionary["+"] = OPERATORS;
	dictionary["-"] = OPERATORS;
	dictionary["/"] = OPERATORS;
	dictionary["*"] = OPERATORS;
	dictionary["="] = OPERATORS;
	dictionary[">"] = OPERATORS;
	dictionary["<"] = OPERATORS;
	dictionary["<="] = OPERATORS;
	dictionary[">="] = OPERATORS;
	dictionary["=="] = OPERATORS;
	dictionary["!="] = OPERATORS;
	dictionary["["] = OPERATORS;
	dictionary["]"] = OPERATORS;
	dictionary["!"] = OPERATORS;
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
	
	// action on current state
	if (input == "")
		throw StateMachineException("sequence of input characters can't be null");

	for (int i = 0; i < input.length(); ++i) {
		if (input[i] == '\n')
			++currentLine;

		// maybe need refactor
		if (input[i] == '"')
			isString = !isString;

		if (IsDelimiter(input[i]) || IsOperator(input[i]) && !isString)
			currentState = DEFINETOKEN;

		if (IsOperator(input[i])) {
			std::string temp(1, input[i]);
			if (IsOperator(input[i + 1]))
				temp += input[i + 1];

			TryCheckToken(readData);
			TryCheckToken(temp);

			readData = "";
			continue;
		}

		if (currentState == GETCHAR)
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

	if (dictionary.find(tokenStr) != dictionary.end()) {
		// we found keyword
		switch (dictionary[tokenStr]){
			case KEYWORDS:
				token.SetName("keyword");
				
				if (tokenStr == "FUNC")
					isFunc = true;
				else
					isFunc = false;

				token.SetCodeData(tokenStr);
				token.SetLine(currentLine);
				break;
			case OPERATORS:
				token.SetName("operator");
				
				token.SetCodeData(tokenStr);
				token.SetLine(currentLine);
				break;
			case DELIMITERS:
				token.SetName("delimiter");
				
				token.SetCodeData(tokenStr);
				token.SetLine(currentLine);
				break;
			case MODIFIER:
				token.SetName("modifier");
				token.SetValue("CONST");
				
				token.SetCodeData(tokenStr);
				token.SetLine(currentLine);
				break;
			default:
				break;
		}
		
		currentState = GETCHAR;

		result->push_back(token);
		return true;
	}
	// maybe need refactor
	else if (IsId(tokenStr) || IsNumber(tokenStr)) {
		bool search = false;
		std::string id;

		if (IsId(tokenStr))
			token.SetName("ID");
		else
			token.SetName("CONSTVAL");

		token.SetCodeData(tokenStr);
		token.SetLine(currentLine);

		for (auto tok : *result)
			if (tok.GetCodeData() == token.GetCodeData()) {
				search = true;
				id = tok.GetValue();
			}

		if (isFunc && funcTable.find(tokenStr) == funcTable.end()) {
			funcTable[tokenStr] = std::to_string(++funcID);
			token.SetValue(std::to_string(funcID));
			isFunc = false;

			return true;
		}

		if (search)
			token.SetValue(id);
		else {
			if (IsId(tokenStr)) {
				token.SetValue(std::to_string(++uniqID));
				if(funcTable.find(tokenStr) == funcTable.end())
					idTable[tokenStr] = std::to_string(uniqID);
			}
			else {
				token.SetValue(std::to_string(++constNumID));
				valTable[tokenStr] = std::to_string(constNumID);
			}
		}

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
	std::string delim(1, ch);

	if (dictionary.find(delim) != dictionary.end())
		if(dictionary[delim] == DELIMITERS)
			return true;

	return false;
}

bool StateMachine::IsOperator(char ch) {
	std::string oper(1, ch);

	if (dictionary.find(oper) != dictionary.end())
		if (dictionary[oper] == OPERATORS)
			return true;

	return false;
}

bool StateMachine::IsNumber(std::string data) {
	for (int i = 0; i < data.length(); ++i)
		if (!IsDigit(data[i]))
			return false;

	return true;
}


bool StateMachine::IsId(std::string data) {
	if ((data[0] == '_' || IsChar(data[0]))) {
		for (int i = 0; i < data.length(); ++i)
			if ((IsOperator(data[i]) || IsDelimiter(data[i])) && data[i] != '-')
				return false;

		return true;
	}

	return false;
}

std::map<std::string, std::string>& StateMachine::GetMapId() {
	return idTable;
}

std::map<std::string, std::string>& StateMachine::GetMapVal() {
	return valTable;
}

std::map<std::string, std::string>& StateMachine::GetMapFunc() {
	return funcTable;
}
