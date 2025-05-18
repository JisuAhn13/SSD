#include "gmock/gmock.h"
#include "CommandChecker.h"
#include <string>
using namespace testing;

TEST(CommandCheckerTest, InvalidRange)
{
	CommandChecker checker;

	EXPECT_FALSE(checker.isValidRange(100));
}

TEST(CommandCheckerTest, ValidRange)
{
	CommandChecker checker;

	EXPECT_TRUE(checker.isValidRange(10));
}

TEST(CommandCheckerTest, InvalidOperator)
{
	CommandChecker checker;

	std::string op = "E";

	EXPECT_FALSE(checker.isValidOperator(op));
}

TEST(CommandCheckerTest, WriteOperator)
{
	CommandChecker checker;

	std::string op = "W";

	EXPECT_TRUE(checker.isValidOperator(op));
}

TEST(CommandCheckerTest, ReadOperator)
{
	CommandChecker checker;

	std::string op = "R";

	EXPECT_TRUE(checker.isValidOperator(op));
}

TEST(CommandCheckerTest, InvalidAddressPrefix)
{
	CommandChecker checker;

	std::string addr = "12345678";

	EXPECT_FALSE(checker.isValidAddress(addr));
}

TEST(CommandCheckerTest, InvalidLongAddress)
{
	CommandChecker checker;

	std::string addr = "0x12345678ab";

	EXPECT_FALSE(checker.isValidAddress(addr));
}

TEST(CommandCheckerTest, InvalidShortAddress)
{
	CommandChecker checker;

	std::string addr = "0x1234";

	EXPECT_FALSE(checker.isValidAddress(addr));
}

TEST(CommandCheckerTest, ValidAddress)
{
	CommandChecker checker;

	std::string addr = "0x1234abcd";

	EXPECT_TRUE(checker.isValidAddress(addr));
}
