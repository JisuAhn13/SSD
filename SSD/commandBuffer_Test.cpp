#include "gmock/gmock.h"
#include "commandBuffer.h"
#include <windows.h>
#include <algorithm>
using namespace testing;

class CmdBufferFixture : public ::testing::Test {
protected:
    void SetUp() override {
        cmdBuffer.clearVec();
        cmdBuffer.clearDir();
    }

    // 파일이 존재하는지 확인하는 함수
    bool fileExists(const std::string& path) {
        DWORD fileAttr = GetFileAttributesA(path.c_str());
        return fileAttr != INVALID_FILE_ATTRIBUTES;
    }

    void feelCmdBuffer() {
        BufferCommand CMD1 = { 'E', 1, 3 };
        BufferCommand CMD2 = { 'E', 3, 5 };
        BufferCommand CMD3 = { 'W', 2, 0x12345678 };
        BufferCommand CMD4 = { 'W', 4, 0x23456789 };
        BufferCommand CMD5 = { 'E', 5, 7 };

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

    CommandBuffer cmdBuffer;
};

void createTestFile(const std::string& fileName, const std::string& content = "") {
    std::string directoryPath = "buffer";
    std::string filePath = directoryPath + "\\" + fileName;
    if (GetFileAttributesA(directoryPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        if (!CreateDirectoryA(directoryPath.c_str(), NULL)) {
            return;
        }
    }
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << content;
        outFile.close();
    }
}

TEST_F(CmdBufferFixture, FilesCreatedCorrectly) {
    std::vector<std::string> bufferFileLists = cmdBuffer.getFileNamesInDirectory();

    for (const auto& fileName : bufferFileLists) {
        ASSERT_TRUE(!fileName.empty());
    }
}

TEST_F(CmdBufferFixture, BufferInitialReadOperation) {

    createTestFile("1_W_99_ABCDEF12.txt");
    createTestFile("2_E_11_13.txt");
    createTestFile("3_W_12_12345678.txt");
    createTestFile("4_E_12_1.txt");
    createTestFile("5_W_19_ACACCABA.txt");

    //creator test purpose 
    CommandBuffer cmdBuf;
    BufferCommand ret = cmdBuf.getBufferIndex(0);
    EXPECT_EQ('W', ret.op);
    EXPECT_EQ(99, ret.firstData);
    EXPECT_EQ(0xABCDEF12, ret.secondData);
}

TEST_F(CmdBufferFixture, full) {
    std::vector<BufferCommand> write_commands = { {CMD_WRITE, 1, 1}, {CMD_WRITE, 2, 2}, {CMD_WRITE, 3, 3}, {CMD_WRITE, 4, 4}, {CMD_WRITE, 5, 5} };

    for (auto c : write_commands) {
        cmdBuffer.enqueue(c);
    }

    EXPECT_TRUE(cmdBuffer.isFull());
}

TEST_F(CmdBufferFixture, notFull) {
    std::vector<BufferCommand> write_commands = { {CMD_WRITE, 1, 1}, {CMD_WRITE, 2, 2}, {CMD_WRITE, 3, 3} };

    for (auto c : write_commands) {
        cmdBuffer.enqueue(c);
    }

    EXPECT_FALSE(cmdBuffer.isFull());
}

TEST_F(CmdBufferFixture, readWithoutOutputFile) {
    std::vector<BufferCommand> write_commands = { {CMD_WRITE, 1, 1}, {CMD_WRITE, 2, 2}, {CMD_WRITE, 3, 3}, {CMD_WRITE, 4, 4}, {CMD_WRITE, 5, 5} };

    for (auto c : write_commands) {
        cmdBuffer.enqueue(c);
    }

    BufferCommand cmd{ CMD_READ, 4, 0 };
    cmdBuffer.enqueue(cmd);
}

TEST_F(CmdBufferFixture, FileWriteSuccess) {
    std::vector<std::string> filename = WriteandReturn();

    EXPECT_EQ(filename[0], "1_E_1_3.txt");
    EXPECT_EQ(filename[1], "2_E_3_5.txt");
    EXPECT_EQ(filename[2], "3_W_2_12345678.txt");
    EXPECT_EQ(filename[3], "4_W_4_23456789.txt");
    EXPECT_EQ(filename[4], "5_E_5_7.txt");
}