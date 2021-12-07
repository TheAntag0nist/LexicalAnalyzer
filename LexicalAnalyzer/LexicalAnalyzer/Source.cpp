#include <iostream>

// our libs
#include "LexicAnalyzer.h"

using namespace std;

int main() {
	LexicAnalyzer analyzer;
	string path = "C:\\Users\\teoru\\source\\repos\\LexicalAnalyzer\\Tests\\";
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