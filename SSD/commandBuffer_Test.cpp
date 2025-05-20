#include "gmock/gmock.h"
#include "commandBuffer.h"
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

    CommandBuffer buffer;
};

TEST_F(CmdBufferFixture, FilesCreatedCorrectly) {
    std::string baseDir = "buffer";
    for (int i = 1; i <= 5; ++i) {
        std::string filePath = baseDir + "\\" + std::to_string(i) + "_empty.txt";
        ASSERT_TRUE(fileExists(filePath)) << "File " << filePath << " does not exist.";
    }
}

TEST_F(CmdBufferFixture, full) {
    std::vector<command> write_commands = { {'W', 1, 1}, {'W', 2, 2}, {'W', 3, 3}, {'W', 4, 4}, {'W', 5, 5} };

    for (auto c : write_commands) {
        buffer.enqueue(c);
    }

    EXPECT_TRUE(buffer.full());
}

TEST_F(CmdBufferFixture, notFull) {
    std::vector<command> write_commands = { {'W', 1, 1}, {'W', 2, 2}, {'W', 3, 3} };

    for (auto c : write_commands) {
        buffer.enqueue(c);
    }

    EXPECT_FALSE(buffer.full());
}