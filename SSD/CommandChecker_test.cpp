#include "gmock/gmock.h"
#include "CommandChecker.h"
using namespace testing;

class CommandFixture : public Test {
public:
	void SetUp() override {

	}

	bool RunTest(const std::vector<std::string>& args) {
		std::vector<char*> argv;
		for (auto& it : args) {
			argv.push_back(const_cast<char*>(it.c_str()));
		}
		return checker.execute((int)argv.size(), argv.data());
	}

	std::string op_invalid = "A";
	std::string op_erase = "E";
	std::string op_read = "R";
	std::string op_write = "W";
	std::string op_flush = "F";

	std::string addr_wrong_format = "12345678";
	std::string addr_short = "0x1234";
	std::string addr_long = "0x12345678ab";
	std::string addr = "0x1234abcd";

	std::string lba_invalid = "100";
	std::string lba = "1";
	std::string lba_size = "5";
	std::string lba_size_large = "11";

	std::string exe = "ssd.exe";

	CommandChecker checker;
};

TEST_F(CommandFixture,InvalidOperator)
{
	std::vector<std::string> args{ exe, op_invalid, lba_invalid, addr };

	bool result = RunTest(args);

	EXPECT_FALSE(result);
}

TEST_F(CommandFixture, Read)
{
	std::vector<std::string> args{ exe, op_read, lba };

	bool result = RunTest(args);

	EXPECT_TRUE(result);
}

TEST_F(CommandFixture, ReadInvalidArgc)
{
	std::vector<std::string> args{ exe, op_read, lba, addr };

	bool result = RunTest(args);

	EXPECT_FALSE(result);
}

TEST_F(CommandFixture, ReadInvalidLba)
{
	std::vector<std::string> args{ exe, op_read, lba_invalid };

	bool result = RunTest(args);

	EXPECT_FALSE(result);
}

TEST_F(CommandFixture, Write)
{
	std::vector<std::string> args{ exe, op_write, lba, addr };

	bool result = RunTest(args);

	EXPECT_TRUE(result);
}

TEST_F(CommandFixture, WriteInvalidArgc)
{
	std::vector<std::string> args{ exe, op_write, lba };

	bool result = RunTest(args);

	EXPECT_FALSE(result);
}

TEST_F(CommandFixture, WriteInvalidLba)
{
	std::vector<std::string> args{ exe, op_write, lba_invalid, addr };

	bool result = RunTest(args);

	EXPECT_FALSE(result);
}

TEST_F(CommandFixture, WriteWrongAddrFormat)
{
	std::vector<std::string> args{ exe, op_write, lba, addr_wrong_format };

	bool result = RunTest(args);

	EXPECT_FALSE(result);
}

TEST_F(CommandFixture, WriteWrongAddrShort)
{
	std::vector<std::string> args{ exe, op_write, lba, addr_short };

	bool result = RunTest(args);

	EXPECT_FALSE(result);
}

TEST_F(CommandFixture, WriteWrongAddrLong)
{
	std::vector<std::string> args{ exe, op_write, lba, addr_long };

	bool result = RunTest(args);

	EXPECT_FALSE(result);
}

TEST_F(CommandFixture, Erase)
{
	std::vector<std::string> args{ exe, op_erase, lba, lba_size };

	bool result = RunTest(args);

	EXPECT_TRUE(result);
}

TEST_F(CommandFixture, EraseInvalidArgc)
{
	std::vector<std::string> args{ exe, op_erase, lba };

	bool result = RunTest(args);

	EXPECT_FALSE(result);
}

TEST_F(CommandFixture, EraseInvalidLba)
{
	std::vector<std::string> args{ exe, op_erase, lba_invalid, lba_size };

	bool result = RunTest(args);

	EXPECT_FALSE(result);
}

TEST_F(CommandFixture, EraseILargeLbaSize)
{
	std::vector<std::string> args{ exe, op_erase, lba, lba_size_large };

	bool result = RunTest(args);

	EXPECT_TRUE(result);
}

TEST_F(CommandFixture, Flush)
{
	std::vector<std::string> args{ exe, op_flush };

	bool result = RunTest(args);

	EXPECT_TRUE(result);
}

TEST_F(CommandFixture, FlushInvalidArgc)
{
	std::vector<std::string> args{ exe, op_flush, lba };

	bool result = RunTest(args);

	EXPECT_FALSE(result);
}
