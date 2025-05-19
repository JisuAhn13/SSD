#include "gmock/gmock.h"
#include "CommandChecker.h"
#include <string>
#include <fstream>
using namespace testing;

class CommandFixture : public Test {
public:
	void SetUp() override {

	}

	const std::string op_invalid = "E";
	const std::string op_read = "R";
	const std::string op_write = "W";

	const std::string addr_wrong_format = "12345678";
	const std::string addr_short = "0x1234";
	const std::string addr_long = "0x12345678ab";
	const std::string addr = "0x1234abcd";

	const std::string lba_invalid = "100";
	const std::string lba = "1";

	const std::string exe = "ssd.exe";

	CommandChecker checker;
};

TEST_F(CommandFixture,InvalidRange)
{
	EXPECT_FALSE(checker.isValidRange(100));
}

TEST_F(CommandFixture,ValidRange)
{
	EXPECT_TRUE(checker.isValidRange(10));
}

TEST_F(CommandFixture,InvalidOperator)
{
	EXPECT_FALSE(checker.isValidOperator(op_invalid));
}

TEST_F(CommandFixture,WriteOperator)
{
	EXPECT_TRUE(checker.isValidOperator(op_write));
}

TEST_F(CommandFixture,ReadOperator)
{
	EXPECT_TRUE(checker.isValidOperator(op_read));
}

TEST_F(CommandFixture,InvalidAddressPrefix)
{
	EXPECT_FALSE(checker.isValidAddress(addr_wrong_format));
}

TEST_F(CommandFixture,InvalidLongAddress)
{
	EXPECT_FALSE(checker.isValidAddress(addr_long));
}

TEST_F(CommandFixture,InvalidShortAddress)
{
	EXPECT_FALSE(checker.isValidAddress(addr_short));
}

TEST_F(CommandFixture,ValidAddress)
{
	EXPECT_TRUE(checker.isValidAddress(addr));
}

TEST_F(CommandFixture,ExecuteWithInvalidArgc)
{
	int argc = 5;
	char* argv[] = { &exe[0], &op_write[0], &lba[0], &addr[0]};

	EXPECT_FALSE(checker.execute(argc, argv));
}

TEST_F(CommandFixture,ExecuteWithInvalidOperator)
{
	int argc = 4;
	char* argv[] = { &exe[0], &op_invalid[0], &lba[0], &addr[0] };

	EXPECT_FALSE(checker.execute(argc, argv));
}

TEST_F(CommandFixture,ExecuteWithInvalidLba)
{
	int argc = 4;
	char* argv[] = { &exe[0], &op_write[0], &lba_invalid[0], &addr[0]};

	EXPECT_FALSE(checker.execute(argc, argv));
}
