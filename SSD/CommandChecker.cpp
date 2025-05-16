#include "CommandChecker.h"

bool CommandChecker::isValidRange(unsigned int LBA)
{
	if (LBA >= 0 && LBA < 100) {
		return true;
	}

	return false;
}

bool CommandChecker::isValidOperator(std::string op)
{
	if (op == "W" || op == "R") {
		return true;
	}

	return false;
}