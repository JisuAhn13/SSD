

#include "gmock/gmock.h"
#include "SSD_func.h"
using namespace testing;


TEST(SDDFunctionTest, ReadLBA3) {
    SSD ssd;

    int Val = 0x12345678;
    int LBA = 3;
    int Writed_Val;

    ssd.write(LBA, Val);

    Writed_Val = ssd.read(LBA);

    EXPECT_EQ(Writed_Val, Val);
}

TEST(SDDFunctionTest, ReadLBA5) {
    SSD ssd;

    int Val = 0x12345555;
    int LBA = 5;
    int Writed_Val;

    ssd.write(LBA, Val);

    Writed_Val = ssd.read(LBA);

    EXPECT_EQ(Writed_Val, Val);
}

TEST(SDDFunctionTest, ReadValueChangeLBA) {
    SSD ssd;

    int Val = 0x12347777;
    int LBA = 7;
    int Writed_Val;

    ssd.write(LBA, Val);

    Writed_Val = ssd.read(LBA);

    Val = 0x12377777;
    ssd.write(LBA, Val);

    Writed_Val = ssd.read(LBA);

    EXPECT_EQ(Writed_Val, Val);
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