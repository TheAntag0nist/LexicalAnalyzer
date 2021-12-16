#include "Token.h"
Token::Token(std::string name, std::string value) {
	this->tokenName = name;
	this->value = value;

	this->codeData = "";
	this->line = 0;
}

Token::~Token() {}

void Token::SetName(std::string name) {
	this->tokenName = name;
}

void Token::SetValue(std::string value) {
	this->value = value;
}

void Token::SetCodeData(std::string data) {
	this->codeData = data;
}

void Token::SetLine(int ln) {
	this->line = ln;
}

std::string Token::GetName() {
	return tokenName;
}

std::string Token::GetValue() {
	return value;
}

std::string Token::GetCodeData() {
	return codeData;
}

int Token::GetLine() {
	return line;
}

std::string Token::GetToken() {
	return ("< " + tokenName + ", " 
				 + (value == "" ? "null" : value) + ">");
}

std::string Token::GetCodeInfo() {
	return ("< " + (codeData == "" ? "null" : codeData) + ", "
				 + (line == 0 ? "null": std::to_string(line)) + ">");
}
