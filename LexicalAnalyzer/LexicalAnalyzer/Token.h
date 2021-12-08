#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TOKENS_CLASS {
	VALUE,
	COMMENT,
	KEYWORDS,
	MODIFIER,
	OPERATORS,
	DELIMITERS,
	ASSIGMENTS,
	IDENTIFIERS
};

class Token {
private:
	std::string tokenName;
	std::string value;

	std::string codeData;
	int line;

public:
	Token(std::string name = "", std::string value = "");
	~Token();

	void SetName(std::string name);
	void SetValue(std::string value);

	void SetCodeData(std::string data);
	void SetLine(int ln);

	std::string GetName();
	std::string GetValue();

	std::string GetCodeData();
	int GetLine();

	std::string GetToken();
	std::string GetCodeInfo();

};

#endif
