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
	machine.SetInput(data);
	machine.SetResultList(&tokens);

	DisplayResults();

	machine.Action();
	syntax.Action(machine, &tokens);
}

void Parser::DisplayResults() {
#ifdef _DEBUG
	int counter = 0;
	bool next = false;

	std::cout << "[INF]:> Analyzer results: " << std::endl;
	// display header
	for(auto token : tokens){
		std::cout << std::setw(24) << token.GetCodeInfo() << "\t"
				  << std::setw(24) << token.GetToken() << std::endl;
	}

	auto mapId = machine.GetMapId();
	auto mapVal = machine.GetMapVal();
	auto mapFunc = machine.GetMapFunc();

	if(mapId.size() > 0)
		std::cout << "[INF]:> ID table: " << std::endl;
	counter = 0;
	for (auto it = mapId.begin(); it != mapId.end(); ++it) {
		std::cout << std::setw(24) << counter << "\t"
			<< std::setw(24) << it->first << std::setw(16) << it->second
			<< std::endl;

		++counter;
	}

	if(mapVal.size() > 0)
		std::cout << "[INF]:> CONST VALUE table: " << std::endl;
	counter = 0;
	for (auto it = mapVal.begin(); it != mapVal.end(); ++it) {
		std::cout << std::setw(24) << counter << "\t"
			<< std::setw(24) << it->first << std::setw(16) << it->second
			<< std::endl;

		++counter;
	}

	if(mapFunc.size() > 0)
		std::cout << "[INF]:> FUNC table: " << std::endl;
	counter = 0;
	for (auto it = mapFunc.begin(); it != mapFunc.end(); ++it) {
		std::cout << std::setw(24) << counter << "\t"
			<< std::setw(24) << it->first << std::setw(16) << it->second
			<< std::endl;

		++counter;
	}

#endif
}