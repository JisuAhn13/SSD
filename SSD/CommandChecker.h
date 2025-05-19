#pragma once
#include <string>

class CommandChecker {
public:
	void execute(int argc, char* argv[]);
	bool isValidRange(unsigned int LBA);
	bool isValidOperator(std::string op);
	bool isValidAddress(std::string addr);
};
