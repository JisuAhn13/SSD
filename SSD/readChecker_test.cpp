#include "gmock/gmock.h"
#include "SSD_func.h"

using namespace testing;

class SSDReadFunctionTest : public Test {
protected:
    std::unique_ptr<SSD> ssd;

    void SetUp() override {
        ssd = std::make_unique<SSD>();
    }

    void writeAndCheck(uint lba, uint value) {
        ssd->write(lba, value);
        EXPECT_EQ(ssd->read(lba), value);
    }
};

TEST_F(SSDReadFunctionTest, ReadLBA3) {
    writeAndCheck(3, 0x12345678);
}

TEST_F(SSDReadFunctionTest, ReadLBA5) {
    writeAndCheck(5, 0x12345555);
}

TEST_F(SSDReadFunctionTest, ReadValueChangeLBA) {
    uint lba = 7;
    ssd->write(lba, 0x12347777);


    ssd->write(lba, 0x12377777);
    EXPECT_EQ(ssd->read(lba), 0x12377777);
}

TEST_F(SSDReadFunctionTest, ReadUnwrittenLBA20) {
    writeAndCheck(20, 0x00000000);
}

TEST_F(SSDReadFunctionTest, ReadOutputFileAndCompare) {
    uint lba = 15;
    uint value = 0x15151515;

    ssd->write(lba, value);
    EXPECT_EQ(ssd->read(lba), value);

    std::ifstream inFile("ssd_output.txt");
    ASSERT_TRUE(inFile.is_open()) << "Failed to open output file: ssd_output.txt";

    std::string line;
    ASSERT_TRUE(std::getline(inFile, line)) << "Output file is empty or unreadable";
    inFile.close();

    EXPECT_EQ(line, "0x15151515");
}