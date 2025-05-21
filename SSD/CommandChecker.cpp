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

	if (isValidOperator(op) == false) {
		return false;
	}

	if (isValidRange(argv) == false) {
		writeOutputFile();
		return false;
	}

	if (op == op_write) {
		std::string lba = std::string(argv[2]);
		std::string addr = std::string(argv[3]);
		return executeWrite(lba, addr);
	}

	if (op == op_read) {
		std::string lba = std::string(argv[2]);
		return executeRead(lba);
	}

	if (op == op_erase) {
		std::string lba = std::string(argv[2]);
		std::string size = std::string(argv[3]);
		return executeErase(lba, size);
	}

	if (op == op_flush) {
		return executeFlush();
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
	int end_lba = start_lba + size_lba - 1;

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
	cmd.execute();

	return true;
}

bool CommandChecker::executeFlush()
{
	FlushCommand cmd;
	cmd.execute();

	return true;
}

bool CommandChecker::isValidRange(char* argv[])
{
	std::string op = std::string(argv[1]);
	if (op == op_flush) {
		return true;
	}

	std::string lba = std::string(argv[2]);
	int lba_value = stoi(lba);
	if (lba_value < 0 || lba_value > 99) {
		return false;
	}

	return true;
}

bool CommandChecker::isValidOperator(std::string op)
{
	if (op == "W" || op == "R" || op == "E" || op == "F") {
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
	CommandBuffer buffer;
	BufferCommand cmd{ 'R', __lba, 0 };
	buffer.enqueue(cmd);
}

void WriteCommand::execute()
{
	CommandBuffer buffer;
	BufferCommand cmd{ 'W', __lba, __addr };
	buffer.enqueue(cmd);
}

void EraseCommand::execute()
{
	CommandBuffer buffer;
	BufferCommand cmd{ 'E', __lba, __lba + __size - 1 };
	buffer.enqueue(cmd);
}

void FlushCommand::execute()
{
	CommandBuffer buffer;
	BufferCommand cmd{ 'F', __lba, 0 };
	buffer.enqueue(cmd);
}