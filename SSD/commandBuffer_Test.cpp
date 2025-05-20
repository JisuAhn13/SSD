#include "gmock/gmock.h"
#include "commandBuffer.h"
#include <windows.h>
#include <algorithm>
using namespace testing;

class CmdBufferFixture : public ::testing::Test {
protected:
    void SetUp() override {
    }

    // 파일이 존재하는지 확인하는 함수
    bool fileExists(const std::string& path) {
        DWORD fileAttr = GetFileAttributesA(path.c_str());
        return (fileAttr != INVALID_FILE_ATTRIBUTES && !(fileAttr & FILE_ATTRIBUTE_DIRECTORY));
    }

    void feelCmdBuffer() {
        command CMD1 = { 'E', 1, 3 };
        command CMD2 = { 'E', 3, 5 };
        command CMD3 = { 'W', 2, 0x12345678 };
        command CMD4 = { 'W', 4, 0x23456789 };
        command CMD5 = { 'E', 5, 7 };

        cmdBuffer.clearVec();
        cmdBuffer.pushCMD(CMD1);
        cmdBuffer.pushCMD(CMD2);
        cmdBuffer.pushCMD(CMD3);
        cmdBuffer.pushCMD(CMD4);
        cmdBuffer.pushCMD(CMD5);
    }

    std::vector<std::string> WriteandReturn() {
        std::vector<std::string> filename;
        
        feelCmdBuffer();

        cmdBuffer.fileWrite();
        
        filename = cmdBuffer.getFileNamesInDirectory();
        
        sort(filename.begin(), filename.end());

        return filename;
    }
private:
    CommandBuffer cmdBuffer;
};

TEST_F(CmdBufferFixture, FilesCreatedCorrectly) {
    std::string baseDir = "buffer";
    for (int i = 1; i <= 5; ++i) {
        std::string filePath = baseDir + "\\" + std::to_string(i) + "_empty.txt";
        ASSERT_TRUE(fileExists(filePath)) << "File " << filePath << " does not exist.";
    }
}

TEST_F(CmdBufferFixture, full) {
    std::vector<command> write_commands = { {CMD_WRITE, 1, 1}, {CMD_WRITE, 2, 2}, {CMD_WRITE, 3, 3}, {CMD_WRITE, 4, 4}, {CMD_WRITE, 5, 5} };

    for (auto c : write_commands) {
        buffer.enqueue(c);
    }

    EXPECT_TRUE(buffer.full());
}

TEST_F(CmdBufferFixture, notFull) {
    std::vector<command> write_commands = { {CMD_WRITE, 1, 1}, {CMD_WRITE, 2, 2}, {CMD_WRITE, 3, 3} };

    for (auto c : write_commands) {
        buffer.enqueue(c);
    }

    EXPECT_FALSE(buffer.full());
}

TEST_F(CmdBufferFixture, readWithoutOutputFile) {
    std::vector<command> write_commands = { {CMD_WRITE, 1, 1}, {CMD_WRITE, 2, 2}, {CMD_WRITE, 3, 3}, {CMD_WRITE, 4, 4}, {CMD_WRITE, 5, 5} };

    for (auto c : write_commands) {
        buffer.enqueue(c);
    }

    command cmd{ CMD_READ, 4, 0 };
    buffer.enqueue(cmd);
}

TEST_F(CmdBufferFixture, FileWriteSuccess) {
    std::vector<std::string> filename = WriteandReturn();

    std::cout << filename[0] << std::endl;
    std::cout << filename[1] << std::endl;

    EXPECT_EQ(filename[0], "1_E_1_3.txt");
    EXPECT_EQ(filename[1], "2_E_3_5.txt");
    EXPECT_EQ(filename[2], "3_W_2_12345678.txt");
    EXPECT_EQ(filename[3], "4_W_4_23456789.txt");
    EXPECT_EQ(filename[4], "5_E_5_7.txt");
}