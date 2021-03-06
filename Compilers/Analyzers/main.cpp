#include <iostream>
#include <windows.h>

#pragma execution_character_set( "utf-8" )

// our libs
#include "Parser.h"

using namespace std;

int main() {
	SetConsoleOutputCP(65001);

	Parser analyzer;
    string path = "";
	string filename;

	cout << "[ENT]:> Enter file for lexic analyze: ";
	getline(cin, filename);

	try {
		// read file
		analyzer.ReadFile( path + filename);
		// analyze
		analyzer.Analyze();
	}
	catch (std::exception& ex) {
		cout << "[ERR]:> " << ex.what() << endl;
		system("pause>NULL");
		return 1;
	}

	cout << "[INF]:> END WORK;" << endl;

	system("pause");
	return 0;
}
