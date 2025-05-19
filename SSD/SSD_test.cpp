#include "gmock/gmock.h"
#include "SSD_func.h"
#include <regex>
using namespace testing;

class SSDReadFunctionTest : public Test {
protected:
    std::unique_ptr<SSD> ssd;
    uint data;
    uint lbaID;

    void SetUp() override {
        ssd = std::make_unique<SSD>();

    }
    void setReadTestValue(uint lbaID, uint data) {
        this->lbaID = lbaID;
        this->data = data;
    }
};

TEST_F(SSDReadFunctionTest, ReadLBA3) {
    setReadTestValue(3, 0x12345678);
    
    ssd->write(lbaID, data);

    EXPECT_EQ(ssd->read(lbaID), data);
}

TEST_F(SSDReadFunctionTest, ReadLBA5) {
    setReadTestValue(5, 0x12345555);

    ssd->write(lbaID, data);

    EXPECT_EQ(ssd->read(lbaID), data);
}

TEST_F(SSDReadFunctionTest, ReadValueChangeLBA) {
    setReadTestValue(7, 0x12347777);
    ssd->write(lbaID, data);

    //it will work on ssd_nand.txt file 
#ifndef UNIT_TEST_WITHOUT_WRITE
    EXPECT_EQ(ssd->read(lbaID), data);
#endif
    setReadTestValue(7, 0x12377777);

    ssd->write(lbaID, data);
    EXPECT_EQ(ssd->read(lbaID), data);
}

TEST_F(SSDReadFunctionTest, InvalideLBAFails) {
    setReadTestValue(100, 0x12345678);

    ssd->write(lbaID, data);

    EXPECT_THROW(ssd->read(lbaID), std::exception);
}

TEST_F(SSDReadFunctionTest, readOutputFileAndCompare) {
    setReadTestValue(15, 0x15151515);

    ssd->write(lbaID, data);
    EXPECT_EQ(ssd->read(lbaID), data);

    std::ifstream inFile("ssd_output.txt");
    ASSERT_TRUE(inFile.is_open()) << "Failed to open output file: ssd_output.txt";

    std::string line;
    ASSERT_TRUE(std::getline(inFile, line)) << "Output file is empty or unreadable";

    inFile.close();

    EXPECT_EQ(line, "0x15151515");
}

TEST_F(SSDReadFunctionTest, ReadUnwrittenLBA20) {
    setReadTestValue(20, 0x00000000);

    ssd->write(lbaID, data);

    EXPECT_EQ(ssd->read(lbaID), data);
}

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

TEST(SDDFunctionTest, InitSSDSuccess) {
    SSD ssd;
    const std::string filename = ssd.getDataFileName();

    ASSERT_TRUE(fileExists(filename));
}

TEST(SDDFunctionTest, WriteSuccess) {
    SSD ssd;

    unsigned int Val = 0x12345678;
    unsigned int LBA = 3;

    const std::string filename = ssd.getDataFileName();

    ssd.write(LBA, Val);

    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());

    std::string line;
    int current = 0;
    while (std::getline(file, line)) {
        if (current == LBA) break;
        ++current;
    }

    ASSERT_EQ(current, LBA);
    EXPECT_EQ(line, "03 0x12345678");
}

TEST(FileFormatTest, CheckFormatSuccess) {
    SSD ssd;
    const std::string line = getLineFromFile(ssd.getDataFileName(), 76);


    ASSERT_FALSE(line.empty());
    EXPECT_TRUE(checkFormat(line));
}