#include "gmock/gmock.h"
#include "SSD_func.h"
#include <regex>
using namespace testing;

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

bool checkFormat(const std::string& line) {
    std::regex pattern(R"(^\d{2} 0x[0-9A-Fa-f]{8}$)");
    return std::regex_match(line, pattern);
}

std::string getLineFromFile(const std::string& filename, int lineNumber) {
    std::ifstream file(filename);
    std::string line;
    int current = 0;
    while (std::getline(file, line)) {
        if (current == lineNumber) return line;
        ++current;
    }
    return "";
}

TEST(SDDFunctionTest, ReadSuccess) {
    SSD ssd;

    int Val = 0x12345678;
    int LBA = 3;
    int Writed_Val;

    ssd.write(LBA, Val);

    Writed_Val = ssd.read(LBA);

    EXPECT_EQ(Writed_Val, Val);
}

TEST(SDDFunctionTest, InitWriteSuccess) {
    const std::string filename = "ssd_nand.txt";
    SSD ssd;

    if (fileExists(filename)) {
        std::remove(filename.c_str());
    }

    ssd.initwrite();

    ASSERT_TRUE(fileExists(filename));
}

TEST(SDDFunctionTest, WriteSuccess) {
    SSD ssd;

    unsigned int Val = 0x12345678;
    unsigned int LBA = 3;

    const std::string filename = "ssd_nand.txt";

    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());

    std::string line;
    int current = 0;
    while (std::getline(file, line)) {
        if (current == 2) break;
        ++current;
    }

    ASSERT_EQ(current, 2);
    EXPECT_EQ(line, "02 0x12345678");
}

TEST(FileFormatTest, CheckFormatSuccess) {
    const std::string line = getLineFromFile("ssd_nand.txt", 76);
    
    
    ASSERT_FALSE(line.empty());
    EXPECT_TRUE(checkFormat(line));
}