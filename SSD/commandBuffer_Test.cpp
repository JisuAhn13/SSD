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
        return fileAttr != INVALID_FILE_ATTRIBUTES;
    }
    CommandBuffer cmdBuf;
};

// 파일 생성 함수 (buffer 경로 밑에 생성)
void createTestFile(const std::string& fileName, const std::string& content = "") {
    // 'buffer' 디렉토리 경로와 파일 이름을 합쳐서 전체 경로를 만듬
    std::string directoryPath = "buffer";
    std::string filePath = directoryPath + "\\" + fileName;

    // buffer 디렉토리가 존재하지 않으면 생성
    if (GetFileAttributesA(directoryPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        // 디렉토리 없으면 생성
        if (!CreateDirectoryA(directoryPath.c_str(), NULL)) {
            std::cerr << "Failed to create directory: " << directoryPath << std::endl;
            return;
        }
    }

    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << content;  // 파일 내용 쓰기
        outFile.close();
        std::cout << "Created file: " << filePath << std::endl;
    }
    else {
        std::cerr << "Failed to create file: " << filePath << std::endl;
    }
}
TEST_F(CmdBufferFixture, FilesCreatedCorrectly) {
    std::vector<std::string> bufferFileLists = cmdBuf.getFileNamesInDirectory();

    for (const auto& fileName : bufferFileLists) {
        ASSERT_TRUE(!fileName.empty());
    }
}

TEST_F(CmdBufferFixture, BufferInitialReadOperation) {
    // 1. W_99_ABCDEF12.txt
    createTestFile("1_W_99_ABCDEF12.txt");

    CommandBuffer::CommandBuffer();
    Command ret= cmdBuf.getBufferIndex(1);
    EXPECT_EQ('W', ret.op);
    EXPECT_EQ(99, ret.firstData);
    EXPECT_EQ(0xABCDEF12, ret.secondData);
        
}