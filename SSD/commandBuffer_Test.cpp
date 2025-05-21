#include "gmock/gmock.h"
#include "commandBuffer.h"

using namespace testing;

class CmdBufferFixture : public ::testing::Test {
protected:
    void SetUp() override {
        cmdBuf.clearVec();
        cmdBuf.clearDir();
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

        cmdBuf.clearVec();
        cmdBuf.pushCMD(CMD1);
        cmdBuf.pushCMD(CMD2);
        cmdBuf.pushCMD(CMD3);
        cmdBuf.pushCMD(CMD4);
        cmdBuf.pushCMD(CMD5);
    }

    std::vector<std::string> WriteandReturn() {
        std::vector<std::string> filename;

        feelCmdBuffer();

        cmdBuf.fileWrite();

        filename = cmdBuf.getFileNamesInDirectory();

        sort(filename.begin(), filename.end());

        return filename;
    }

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

    CommandBuffer& cmdBuf = CommandBuffer::getInstance();
};

TEST_F(CmdBufferFixture, FilesCreatedCorrectly) {
    std::vector<std::string> bufferFileLists = cmdBuf.getFileNamesInDirectory();

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
    cmdBuf.initializeCommandBuffer();
    BufferCommand ret = cmdBuf.getBufferIndex(0);
    EXPECT_EQ('W', ret.op);
    EXPECT_EQ(99, ret.firstData);
    EXPECT_EQ(0xABCDEF12, ret.secondData);
}

TEST_F(CmdBufferFixture, full) {
    std::vector<BufferCommand> write_commands = { {CMD_WRITE, 1, 1}, {CMD_WRITE, 2, 2}, {CMD_WRITE, 3, 3}, {CMD_WRITE, 4, 4}, {CMD_WRITE, 5, 5} };

    for (auto c : write_commands) {
        cmdBuf.enqueue(c);
    }

    EXPECT_TRUE(cmdBuf.isFull());
}

TEST_F(CmdBufferFixture, notFull) {
    std::vector<BufferCommand> write_commands = { {CMD_WRITE, 1, 1}, {CMD_WRITE, 2, 2}, {CMD_WRITE, 3, 3} };

    for (auto c : write_commands) {
        cmdBuf.enqueue(c);
    }

    EXPECT_FALSE(cmdBuf.isFull());
}

TEST_F(CmdBufferFixture, readWithoutOutputFile) {
    std::vector<BufferCommand> write_commands = { {CMD_WRITE, 1, 1}, {CMD_WRITE, 2, 2}, {CMD_WRITE, 3, 3}, {CMD_WRITE, 4, 4}, {CMD_WRITE, 5, 5} };

    for (auto c : write_commands) {
        cmdBuf.enqueue(c);
    }

    BufferCommand cmd{ CMD_READ, 4, 0 };
    cmdBuf.enqueue(cmd);
}

TEST_F(CmdBufferFixture, FileWriteSuccess) {
    std::vector<std::string> filename = WriteandReturn();

    EXPECT_EQ(filename[0], "1_E_1_3.txt");
    EXPECT_EQ(filename[1], "2_E_3_5.txt");
    EXPECT_EQ(filename[2], "3_W_2_12345678.txt");
    EXPECT_EQ(filename[3], "4_W_4_23456789.txt");
    EXPECT_EQ(filename[4], "5_E_5_7.txt");
}

TEST_F(CmdBufferFixture, CheckEraseAlgorithmRemoveTC1) {
    createTestFile("1_E_1_3.txt");          // E 1~3 → 길이 3
    createTestFile("2_E_3_3.txt");          // E 3~5 → 길이 3
    createTestFile("3_W_2_12345678.txt");   // W 2 = 0x12345678
    createTestFile("4_W_4_23456789.txt");   // W 4 = 0x23456789
    createTestFile("5_E_1_5.txt");          // E 1~5 → 길이 5

    //creator test purpose 
    cmdBuf.initializeCommandBuffer();
    cmdBuf.eraseAlgorithm();
    BufferCommand ret = cmdBuf.getBufferIndex(0);
    EXPECT_EQ('E', ret.op);
    EXPECT_EQ(1, ret.firstData);
    EXPECT_EQ(5, ret.secondData);
}

TEST_F(CmdBufferFixture, CheckEraseAlgorithmRemoveTC2) {
    createTestFile("1_E_1_3.txt");          // E 1~3 → 길이 3
    createTestFile("2_E_3_4.txt");          // E 3~6 → 길이 4
    createTestFile("3_W_2_12345678.txt");   // W 2 = 0x12345678
    createTestFile("4_W_4_23456789.txt");   // W 4 = 0x23456789
    createTestFile("5_E_1_5.txt");          // E 1~5 → 길이 5

    //creator test purpose 
    cmdBuf.initializeCommandBuffer();
    cmdBuf.eraseAlgorithm();
    BufferCommand ret = cmdBuf.getBufferIndex(0);
    EXPECT_EQ('E', ret.op);
    EXPECT_EQ(3, ret.firstData);
    EXPECT_EQ(4, ret.secondData);
}

TEST_F(CmdBufferFixture, CheckMergeEraseAlgorithm) {
    createTestFile("1_E_1_2.txt");          // E 1~2 → 길이 2
    createTestFile("2_E_2_3.txt");          // E 2~4 → 길이 3
    createTestFile("3_W_10_12345678.txt");  // W 10 = 0x12345678
    createTestFile("4_E_15_1.txt");         // E 15 → 길이 1
    createTestFile("5_E_16_2.txt");         // E 16~17 → 길이 2

    //creator test purpose 
    cmdBuf.initializeCommandBuffer();
    cmdBuf.mergeAlgorithm();
    BufferCommand ret = cmdBuf.getBufferIndex(0);
    EXPECT_EQ('E', ret.op);
    EXPECT_EQ(1, ret.firstData);
    EXPECT_EQ(4, ret.secondData);

    ret = cmdBuf.getBufferIndex(1);
    EXPECT_EQ('W', ret.op);
    EXPECT_EQ(10, ret.firstData);
    EXPECT_EQ(0x12345678, ret.secondData);

    ret = cmdBuf.getBufferIndex(2);
    EXPECT_EQ('E', ret.op);
    EXPECT_EQ(15, ret.firstData);
    EXPECT_EQ(3, ret.secondData);
}