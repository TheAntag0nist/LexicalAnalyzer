#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TOKENS_CLASS {
	KEYWORDS,
	DELIMITERS,
	OPERATORS,
	ASSIGMENTS,
	IDENTIFIERS
};

class Token {
private:
	std::string tokenName;
	std::string value;

public:
	Token();
	~Token();

	void SetName(std::string name);
	void SetValue(std::string value);

	std::string GetName();
	std::string GetValue();

	std::string GetToken();

};

#endif