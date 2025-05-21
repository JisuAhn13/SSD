#include "gmock/gmock.h"
#include "CommandChecker.h"
using namespace testing;

class CommandFixture : public Test {
public:
	void SetUp() override {

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

	std::string exe = "ssd.exe";

	CommandChecker checker;
};

TEST_F(CommandFixture,InvalidRange)
{
	char* argv[] = { &exe[0], &op_write[0], &lba_invalid[0], &addr[0] };
	EXPECT_FALSE(checker.isValidRange(argv));
}

TEST_F(CommandFixture,ValidRange)
{
	char* argv[] = { &exe[0], &op_write[0], &lba[0], &addr[0] };
	EXPECT_TRUE(checker.isValidRange(argv));
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

TEST_F(CommandFixture, ExecuteWrite)
{
	int argc = 4;
	char* argv[] = { &exe[0], &op_write[0], &lba[0], &addr[0]};

	EXPECT_TRUE(checker.execute(argc, argv));
}

TEST_F(CommandFixture, ExecuteRead)
{
	int argc = 3;
	char* argv[] = { &exe[0], &op_read[0], &lba[0] };

	EXPECT_TRUE(checker.execute(argc, argv));
}

TEST_F(CommandFixture, ExecuteErase)
{
	int argc = 4;
	char* argv[] = { &exe[0], &op_erase[0], &lba[0], &lba_size[0]};

	EXPECT_TRUE(checker.execute(argc, argv));
}

TEST_F(CommandFixture, ExecuteFlush)
{
	int argc = 2;
	char* argv[] = { &exe[0], &op_flush[0] };

	EXPECT_TRUE(checker.execute(argc, argv));
}
