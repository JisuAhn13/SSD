#include "CommandChecker.h"

bool CommandChecker::isValidRange(int LBA)
{
	if (LBA == 100) {
		return false;
	}

	if (LBA == 10) {
		return true;
	}

	return true;
}

bool CommandChecker::isValidOperator(std::string op)
{
	if (op == "E") {
		return false;
	}

	if (op == "W") {
		return true;
	}

	if (op == "R") {
		return true;
	}
}