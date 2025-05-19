#include "gmock/gmock.h"
#include "SSD_func.h"
using namespace testing;

class SSDWriteFunctionTest : public Test {
protected:
    SSD ssd;
    std::string filename;

    void SetUp() override {
        filename = ssd.getDataFileName();
    }

    std::string getLineFromFile(int lineNumber) {
        std::ifstream file(filename);
        std::string line;
        int current = 0;
        while (std::getline(file, line)) {
            if (current == lineNumber) return line;
            ++current;
        }
        return "";
    }

    bool fileExists() {
        std::ifstream file(filename);
        return file.good();
    }

    bool checkFormat(const std::string& line) {
        std::regex pattern(R"(^\d{2} 0x[0-9A-Fa-f]{8}$)");
        return std::regex_match(line, pattern);
    }
};

TEST_F(SSDWriteFunctionTest, InitSSDSuccess) {
    ASSERT_TRUE(fileExists());
}

TEST_F(SSDWriteFunctionTest, WriteLineFormatCheck) {
    uint lba = 3;
    uint value = 0x12345678;
    ssd.write(lba, value);

    std::string line = getLineFromFile(lba);
    EXPECT_EQ(line, "03 0x12345678");
}

TEST_F(SSDWriteFunctionTest, FileLineFormatIsCorrect) {
    std::string line = getLineFromFile(76);

    ASSERT_FALSE(line.empty());
    EXPECT_TRUE(checkFormat(line));
}