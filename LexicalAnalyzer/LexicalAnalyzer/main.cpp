#include <iostream>
#include <windows.h>

#pragma execution_character_set( "utf-8" )

// our libs
#include "Parser.h"

using namespace std;

int main() {
	SetConsoleOutputCP(65001);

	Parser analyzer;
	string path = "C:\\";
	string filename;

	cout << "Enter file for lexic analyze: ";
	getline(cin, filename);

	try {
		// read file
		analyzer.ReadFile( path + filename);
		// analyze
		analyzer.Analyze();
		// display messages
		// only in debug
		analyzer.DisplayResults();
	}
	catch (std::exception& ex) {
		cout << "[ERR]:> " << ex.what() << endl;
		return 1;
	}

	system("pause>NULL");
	return 0;
}