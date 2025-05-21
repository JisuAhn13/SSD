#pragma once
#include <string>
#include "SSD_func.h"
#include "commandBuffer.h"

class Command {
public:
	explicit Command(unsigned int lba) : __lba(lba)
	{
	}
	virtual void execute() = 0;

protected:
	unsigned int __lba;
};

class ReadCommand : public Command {
public:
	explicit ReadCommand(unsigned int lba = 0) : Command(lba)
	{
	}
	void execute() override;
};

class WriteCommand : public Command {
public:
	explicit WriteCommand(unsigned int lba = 0, unsigned int addr = 0) : Command(lba), __addr(addr)
	{
	}
	void execute() override;

private:
	unsigned int __addr;
};

class EraseCommand : public Command {
public:
	explicit EraseCommand(unsigned int lba = 0, unsigned int size = 0) : Command(lba), __size(size)
	{
	}
	void execute() override;

private:
	unsigned int __size;
};

class FlushCommand : public Command {
public:
	explicit FlushCommand() : Command(0)
	{
	}

	void execute() override;
};

class CommandChecker {
public:
	bool execute(int argc, char* argv[]);

private:
	bool executeFlush();
	bool executeErase(char* argv[]);
	bool executeRead(char* argv[]);
	bool executeWrite(char* argv[]);

	void writeOutputFile();

	bool isValidOperator(std::string op);
	bool isValidArgc(std::string op, unsigned int argc);
	bool isValidRange(std::string lba);
	bool isValidRange(std::string lba, std::string size);
	bool isValidAddress(std::string addr);
};
