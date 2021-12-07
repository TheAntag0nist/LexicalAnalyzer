#include "Token.h"
Token::Token() {}

Token::~Token() {}

void Token::SetName(std::string name) {
	this->tokenName = name;
}

void Token::SetValue(std::string value) {
	this->value = value;
}

std::string Token::GetName() {
	return tokenName;
}

std::string Token::GetValue() {
	return value;
}

std::string Token::GetToken() {
	return ("< " + tokenName + ", " + value + ">");
}