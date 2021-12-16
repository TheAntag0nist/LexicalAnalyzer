#ifndef ANALYZE_EXCEPTIONS_H
#define ANALYZE_EXCEPTIONS_H

#include <exception>
#include <string>

class ReadException : public std::exception {
private:
	std::string msg;

public:
	ReadException(const std::string& msg) : msg(msg) {}

	virtual const char* what() const noexcept override {
		return msg.c_str();
	}

};

class StateMachineException : public std::exception {
private:
	std::string msg;

public:
	StateMachineException(const std::string& msg) : msg(msg) {}

	virtual const char* what() const noexcept override {
		return msg.c_str();
	}

};

#endif
