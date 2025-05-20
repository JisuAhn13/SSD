#include "CommandChecker.h"
#include "SSD_func.h"
#include <fstream>
#include <regex>

const std::string output_filename = "ssd_output.txt";

void writeOutputFile()
{
	std::ofstream fs;
	fs.open(output_filename);
	fs.clear();
	fs << "ERROR";
	fs.close();
}

bool CommandChecker::execute(int argc, char* argv[])
{
	if (argc > 4 || argc < 2) {
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
		std::string addr = std::string(argv[3]);
		return executeWrite(lba, addr);
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
	unsigned int lba_val = (unsigned int)std::stoi(lba);
	ReadCommand cmd{ lba_val };
	cmd.execute();

	return true;
}

bool CommandChecker::executeWrite(std::string lba, std::string addr)
{
	if (isValidAddress(addr) == false) {
		return false;
	}

	unsigned int lba_val = (unsigned int)std::stoi(lba);
	unsigned int addr_val = (unsigned int)std::stoll(addr.substr(2), nullptr, 16);

	WriteCommand cmd{ lba_val, addr_val };
	cmd.execute();

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

	EraseCommand cmd { (unsigned int)start_lba, (unsigned int)size_lba };

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

void ReadCommand::execute()
{
	__ssd.read(__lba);
}

void WriteCommand::execute()
{
	__ssd.write(__lba, __addr);
}

void EraseCommand::execute()
{
	__ssd.erase(__lba, __lba + __size);
}