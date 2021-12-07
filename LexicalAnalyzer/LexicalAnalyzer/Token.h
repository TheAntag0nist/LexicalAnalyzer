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

	TOKENS_CLASS type;

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