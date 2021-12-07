#include "Parser.h"

Parser::Parser() {}

Parser::~Parser() {}

void Parser::ReadFile(std::string file) {
	std::ifstream inputFile(file);

	if (!inputFile.is_open())
		throw ReadException("file not open");
	else {
		std::stringstream buffer;
		buffer << inputFile.rdbuf();
		data = buffer.str();
	}

#ifdef _DEBUG
	std::cout << "[INF]:> Success reading;" << std::endl << "data = " << data << std::endl;
#endif
}

void Parser::ReadFile(std::ifstream file) {
	if (!file.is_open())
		throw ReadException("file not open");
	else {
		std::stringstream buffer;
		buffer << file.rdbuf();
		data = buffer.str();
	}

#ifdef _DEBUG
		std::cout << "[INF]:> Success reading;" << std::endl << "data = " << data << std::endl;
#endif
}

void Parser::Analyze() {
	// works like finite state machine
	StateMachine machine;

	machine.SetInput(data);
	machine.SetResultList(&tokens);

	machine.Action();
}

void Parser::DisplayResults() {
#ifdef _DEBUG
	std::cout << "[INF]:> Analyzer results: " << std::endl;
	// display header
	for(auto token : tokens){
		std::cout << std::setw(24) << token.GetCodeInfo() << "\t"
				  << std::setw(16) << token.GetToken() << std::endl;
	}
#endif
}