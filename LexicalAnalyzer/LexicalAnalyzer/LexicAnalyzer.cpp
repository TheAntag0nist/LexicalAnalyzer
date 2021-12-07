#include "LexicAnalyzer.h"

LexicAnalyzer::LexicAnalyzer() {}

LexicAnalyzer::~LexicAnalyzer() {}

void LexicAnalyzer::ReadFile(std::string file) {
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

void LexicAnalyzer::ReadFile(std::ifstream file) {
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

void LexicAnalyzer::Analyze() {
	// works like finite state machine
}

void LexicAnalyzer::DisplayResults() {
	for(auto token : tokens){
#ifdef _DEBUG
		std::cout << token.GetToken() << std::endl;
#endif
	}
}