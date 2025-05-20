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
};

TEST_F(CmdBufferFixture, FilesCreatedCorrectly) {
    std::string baseDir = "buffer";
    for (int i = 1; i <= 5; ++i) {
        std::string filePath = baseDir + "\\" + std::to_string(i) + "_empty.txt";
        ASSERT_TRUE(fileExists(filePath)) << "File " << filePath << " does not exist.";
    }
}