#include "CommandChecker.h"
#include "SSD_func.h"
#include <fstream>
#include <regex>

const std::string output_filename = "ssd_output.txt";
const std::string op_read = "R";
const std::string op_write = "W";
const std::string op_erase = "E";
const std::string op_flush = "F";

std::vector<std::string> VALID_CMD{ op_write, op_read, op_erase, op_flush };

void CommandChecker::writeOutputFile()
{
	std::ofstream fs;
	fs.open(output_filename);
	fs.clear();
	fs << "ERROR";
	fs.close();
}

bool CommandChecker::isValidOperator(std::string op)
{
	if (std::find(VALID_CMD.begin(), VALID_CMD.end(), op) == VALID_CMD.end()) {
		return false;
	}

	return true;
}

bool CommandChecker::isValidArgc(std::string op, unsigned int argc)
{
	bool result = true;
	if (op == op_write) {
		result = (argc == 4);
	}
	else if (op == op_read) {
		result = (argc == 3);
	}
	else if (op == op_erase) {
		result = (argc == 4);
	}
	else if (op == op_flush) {
		result = (argc == 2);
	}
	return result;
}

bool CommandChecker::isValidRange(std::string lba)
{
	int lba_value = stoi(lba);
	if (lba_value < 0 || lba_value > 99) {
		writeOutputFile();
		return false;
	}
	return true;
}

bool CommandChecker::isValidRange(std::string lba, std::string size)
{
	if (isValidRange(lba) == false) {
		return false;
	}

	int start_lba = std::stoi(lba);
	int size_lba = std::stoi(size);
	size_lba = (size_lba > 10) ? 10 : size_lba;
	int end_lba = start_lba + size_lba - 1;

	if (size_lba < 0) {
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
}

bool CommandChecker::isValidAddress(std::string addr)
{
	std::regex pattern(R"(^0x[0-9A-Fa-f]{8}$)");
	return std::regex_match(addr, pattern);
}

bool CommandChecker::execute(int argc, char* argv[])
{
	std::string op = std::string(argv[1]);

	if (isValidOperator(op) == false) {
		return false;
	}

	if (isValidArgc(op, argc) == false) {
		return false;
	}

	if (op == op_write) {
		return executeWrite(argv);
	}

	if (op == op_read) {
		return executeRead(argv);
	}

	if (op == op_erase) {

		return executeErase(argv);
	}

	if (op == op_flush) {
		return executeFlush();
	}

	return false;
}

bool CommandChecker::executeFlush()
{
	FlushCommand cmd;
	cmd.execute();

	return true;
}

bool CommandChecker::executeErase(char* argv[])
{
	std::string lba = std::string(argv[2]);
	std::string size = std::string(argv[3]);

	if (isValidRange(lba, size) == false) {
		return false;
	}

	int start_lba = std::stoi(lba);
	int size_lba = std::stoi(size);
	int end_lba = start_lba + size_lba - 1;

	EraseCommand cmd{ (unsigned int)start_lba, (unsigned int)end_lba };
	cmd.execute();

	return true;
}

bool CommandChecker::executeRead(char* argv[])
{
	std::string lba = std::string(argv[2]);
	if (isValidRange(lba) == false) {
		return false;
	}

	unsigned int lba_val = (unsigned int)std::stoi(lba);
	ReadCommand cmd{ lba_val };
	cmd.execute();

	return true;
}

bool CommandChecker::executeWrite(char* argv[])
{
	std::string lba = std::string(argv[2]);
	if (isValidRange(lba) == false) {
		return false;
	}

	std::string addr = std::string(argv[3]);
	if (isValidAddress(addr) == false) {
		return false;
	}

	unsigned int lba_val = (unsigned int)std::stoi(lba);
	unsigned int addr_val = (unsigned int)std::stoll(addr.substr(2), nullptr, 16);

	WriteCommand cmd{ lba_val, addr_val };
	cmd.execute();

	return true;
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
	BufferCommand cmd{ 'E', __lba, __size};
	buffer.enqueue(cmd);
}

void FlushCommand::execute()
{
	CommandBuffer buffer;
	BufferCommand cmd{ 'F', __lba, 0 };
	buffer.enqueue(cmd);
}