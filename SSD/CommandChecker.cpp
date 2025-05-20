#include "CommandChecker.h"
#include "SSD_func.h"
#include <fstream>
#include <regex>

bool CommandChecker::execute(int argc, char* argv[])
{
	if (argc > 4) {
		return false;
	}

	std::string op = std::string(argv[1]);
	std::string lba = std::string(argv[2]);

	if (isValidOperator(op) == false) {
		return false;
	}

	if (isValidRange(std::stoi(lba)) == false) {
		writeOutputFile();
		return false;
	}

	if (op == op_write) {
		std::string value = std::string(argv[3]);
		return executeWrite(lba, value);
	}

	if (op == op_read) {
		return executeRead(lba);
	}

	if (op == op_erase) {
		std::string size = std::string(argv[3]);
		return executeErase(lba, size);
	}

	return false;
}

bool CommandChecker::executeRead(std::string lba)
{
	ssd.read((unsigned int)std::stoi(lba));
	return true;
}

bool CommandChecker::executeWrite(std::string lba , std::string addr)
{
	if (isValidAddress(addr) == false) {
		return false;
	}

	ssd.write((unsigned int)std::stoi(lba), (unsigned int)std::stoll(addr.substr(2), nullptr, 16));

	return true;
}

bool CommandChecker::executeErase(std::string lba, std::string size)
{
	int start_lba = std::stoi(lba);
	int size_lba = std::stoi(size);
	int end_lba = start_lba + size_lba;

	if (start_lba < 0 || size_lba < 0) {
		writeOutputFile();
		return false;
	}

	if (start_lba > 99 || end_lba > 99) {
		writeOutputFile();
		return false;
	}

	if (size_lba == 0) {
		return false;
	}

	if (size_lba > 10) {
		end_lba = start_lba + 9;
	}

	ssd.erase(start_lba, end_lba);

	return true;
}

void CommandChecker::writeOutputFile()
{
	std::ofstream fs;
	fs.open(output_filename);
	fs.clear();
	fs << "ERROR";
	fs.close();
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
	if (op == "W" || op == "R" || op == "E") {
		return true;
	}

	return false;
}

bool CommandChecker::isValidAddress(std::string addr)
{
	std::regex pattern(R"(^0x[0-9A-Fa-f]{8}$)");
	return std::regex_match(addr, pattern);
}