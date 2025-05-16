

#include "gmock/gmock.h"
#include "SSD_func.h"
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

#if 0
TEST(SDDFunctionTest, WriteSuccess) {
    SSD ssd;

    int Val = 0x12345678;
    int LBA = 3;
    int Writed_Val;

    ssd.write(LBA, Val);

    Writed_Val = ssd.read(LBA);

    EXPECT_EQ(Writed_Val, Val);
}
#endif