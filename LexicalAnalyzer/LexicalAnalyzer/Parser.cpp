#include "Parser.h"

Parser::Parser() {}

Parser::~Parser() {}

void Parser::ReadFile(std::string file) {
	std::ifstream inputFile(file);

	if (!inputFile.is_open())
		throw new ReadException("file not open");
	else {
		std::stringstream buffer;
		buffer << inputFile.rdbuf();
		data = buffer.str();
	}

#ifdef _DEBUG
	std::cout << "[INF]:> Success reading;" << std::endl << "data = " << data;
#endif
}

void Parser::ReadFile(std::ifstream file) {
	if (!file.is_open())
		throw new ReadException("file not open");
	else {
		std::stringstream buffer;
		buffer << file.rdbuf();
		data = buffer.str();
	}

#ifdef _DEBUG
		std::cout << "[INF]:> Success reading;" << std::endl << "data = " << data;
#endif
}

void Parser::Analyze() {
	// works like finite state machine
}

void Parser::DisplayResults() {
#ifdef _DEBUG
	for(auto token : tokens){
		std::cout << token.GetToken() << std::endl;
	}
#endif
}