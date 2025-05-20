#include "gmock/gmock.h"
#include "commandBuffer.h"
using namespace testing;

class CmdBufferFixture : public ::testing::Test {
protected:
    void SetUp() override {
    }

    // ������ �����ϴ��� Ȯ���ϴ� �Լ�
    bool fileExists(const std::string& path) {
        DWORD fileAttr = GetFileAttributesA(path.c_str());
        return fileAttr != INVALID_FILE_ATTRIBUTES;
    }
    CommandBuffer cmdBuf;
};

// ���� ���� �Լ� (buffer ��� �ؿ� ����)
void createTestFile(const std::string& fileName, const std::string& content = "") {
    // 'buffer' ���丮 ��ο� ���� �̸��� ���ļ� ��ü ��θ� ����
    std::string directoryPath = "buffer";
    std::string filePath = directoryPath + "\\" + fileName;

    // buffer ���丮�� �������� ������ ����
    if (GetFileAttributesA(directoryPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        // ���丮 ������ ����
        if (!CreateDirectoryA(directoryPath.c_str(), NULL)) {
            std::cerr << "Failed to create directory: " << directoryPath << std::endl;
            return;
        }
    }

    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << content;  // ���� ���� ����
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