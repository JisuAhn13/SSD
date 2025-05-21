#pragma once
#include <string>
#include "SSD_func.h"
#include "commandBuffer.h"

class Command {
public:
	enum Type_e {
		READ,
		WRITE,
		ERASE,
		FLUSH,
	};

	explicit Command(Type_e Type, unsigned int lba) : __Type(Type), __lba(lba)
	{
	}
	virtual void execute() = 0;

	Type_e getType() const {
		return __Type;
	}

	unsigned int getLba() const {
		return __lba;
	}

protected:
	Type_e __Type;
	unsigned int __lba;
};

class ReadCommand : public Command {
public:
	explicit ReadCommand(unsigned int lba) : Command(Command::READ, lba)
	{
	}
	void execute() override;
};

class WriteCommand : public Command {
public:
	explicit WriteCommand(unsigned int lba, unsigned int addr) : Command(Command::WRITE, lba), __addr(addr)
	{
	}
	void execute() override;

	unsigned int getAddr() const {
		return __addr;
	}

private:
	unsigned int __addr;
};

class EraseCommand : public Command {
public:
	explicit EraseCommand(unsigned int lba, unsigned int size) : Command(Command::ERASE, lba), __size(size)
	{
	}
	void execute() override;

	unsigned int getSize() const {
		return __size;
	}

private:
	unsigned int __size;
};

class FlushCommand : public Command {
public:
	explicit FlushCommand() : Command(Command::FLUSH, 0)
	{
	}

	void execute() override;
};

class CommandChecker {
public:
	bool execute(int argc, char* argv[]);

	bool isValidRange(char* argv[]);
	bool isValidOperator(std::string op);
	bool isValidAddress(std::string addr);

private:
	bool executeRead(std::string lba);
	bool executeWrite(std::string lba, std::string addr);
	bool executeErase(std::string lba, std::string size);
	bool executeFlush();

	const std::string output_filename = "ssd_output.txt";

	const std::string op_read = "R";
	const std::string op_write = "W";
	const std::string op_erase = "E";
	const std::string op_flush = "F";
};
