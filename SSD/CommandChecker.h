#pragma once
#include <string>
#include "SSD_func.h"

class CommandChecker {
public:
	bool execute(int argc, char* argv[]);
	void writeOutputFile();
	bool isValidRange(unsigned int LBA);
	bool isValidOperator(std::string op);
	bool isValidAddress(std::string addr);

private:
	bool executeRead(std::string lba);
	bool executeWrite(std::string lba, std::string addr);
	bool executeErase(std::string lba, std::string size);

	const std::string output_filename = "ssd_output.txt";

	const std::string op_read = "R";
	const std::string op_write = "W";
	const std::string op_erase = "E";

	SSD ssd;
};
