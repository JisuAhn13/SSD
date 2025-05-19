#include "CommandChecker.h"
#include "SSD_func.h"
#include <fstream>
#include <regex>

bool CommandChecker::execute(int argc, char* argv[])
{
	std::string op = std::string(argv[1]);
	std::string lba = std::string(argv[2]);
	std::string value = std::string(argv[3]);
	const std::string filename = "ssd_output.txt";

	if (argc > 4) {
		return false;
	}

	if (isValidOperator(op) == false) {
		return false;
	}

	if (isValidRange(std::stoi(lba)) == false) {
		std::ofstream fs;
		fs.open(filename);
		fs.clear();
		fs << "ERROR";
		fs.close();
		return false;
	}

	if (isValidAddress(value) == false) {
		return false;
	}

	SSD ssd;
	std::string op_read = "R";
	std::string op_write = "W";

	if (op == op_read) {
		ssd.read(std::stoi(lba));
	}
	else if (op == op_write) {
		ssd.write(std::stoi(lba), std::stoi(value.substr(2), nullptr, 16));
	}
	else {
		return false;
	}

	return true;
}

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

bool CommandChecker::isValidAddress(std::string addr)
{
	std::regex pattern(R"(^0x[0-9A-Fa-f]{8}$)");
	return std::regex_match(addr, pattern);
}