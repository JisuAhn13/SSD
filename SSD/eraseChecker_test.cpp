#include "gmock/gmock.h"
#include "SSD_func.h"
using namespace testing;

class SSDEraseFunctionTest : public Test {
protected:
    std::unique_ptr<SSD> ssd;

    void SetUp() override {
        ssd = std::make_unique<SSD>();
    }

    void WriteNoneZeroValue(uint start_lba, uint end_lba) {
        for (int lba = start_lba; lba <= end_lba; lba++) {
            ssd->write(lba, 0x12345678);
        }
    }

    void EraseAndCheck(uint start_lba, uint end_lba) {
        WriteNoneZeroValue(start_lba, end_lba);

        ssd->erase(start_lba, end_lba);
        for (int lba = start_lba; lba <= end_lba; lba++) {
            EXPECT_EQ(ssd->read(lba), 0x00000000);
        }
    }
};

TEST_F(SSDEraseFunctionTest, EraseSuccess) {
    EraseAndCheck(5,15);
}

TEST_F(SSDEraseFunctionTest, Only_Erase_LBA_Range) {
    WriteNoneZeroValue(4, 16);
    
    EraseAndCheck(0, 15);
    EXPECT_EQ(ssd->read(4), 0x12345678);
    EXPECT_EQ(ssd->read(16), 0x12345678);
}