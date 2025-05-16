#pragma once
#include <string>

class CommandChecker {
public:
	bool isValidRange(unsigned int LBA);
	bool isValidOperator(std::string op);
	bool isValidAddress(std::string addr);
};
