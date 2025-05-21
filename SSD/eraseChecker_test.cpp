#include "gmock/gmock.h"
#include "SSD_func.h"
using namespace testing;

class SSDEraseFunctionTest : public Test {
protected:
    std::unique_ptr<SSD> ssd;

    void SetUp() override {
        ssd = std::make_unique<SSD>();
    }

    void WriteNoneZeroValue(uint start_lba, uint size) {
        for (int idx = 0; idx < size; idx++) {
            ssd->write(start_lba+idx, 0x12345678);
        }
    }

    void EraseAndCheck(uint start_lba, uint size) {
        WriteNoneZeroValue(start_lba, size);

        ssd->erase(start_lba, size);
        for (int idx = 0; idx < size; idx++) {
            EXPECT_EQ(ssd->read(start_lba+idx), 0x00000000);
        }
    }
};

TEST_F(SSDEraseFunctionTest, EraseSuccess) {
    EraseAndCheck(5,10);
}

TEST_F(SSDEraseFunctionTest, Only_Erase_LBA_Range) {
    WriteNoneZeroValue(4, 13);
    
    EraseAndCheck(5, 10);
    EXPECT_EQ(ssd->read(4), 0x12345678);
    EXPECT_EQ(ssd->read(16), 0x12345678);
}