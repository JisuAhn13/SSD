#include "commandBuffer.h"
#include "SSD_func.h"

// 디렉토리 존재 여부 확인
bool CommandBuffer::directoryExists(const std::string& path) {
    DWORD ftyp = GetFileAttributesA(path.c_str());
    return (ftyp != INVALID_FILE_ATTRIBUTES && (ftyp & FILE_ATTRIBUTE_DIRECTORY));
}

bool CommandBuffer::fileExists(const std::string& path) {
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}

// 생성자 
CommandBuffer::CommandBuffer() {
    std::string baseDir = "buffer";

    // buffer 폴더 생성
    if (!directoryExists(baseDir)) {
        if (!CreateDirectoryA(baseDir.c_str(), NULL)) {
            std::cerr << "Failed to create base directory: " << baseDir << std::endl;
            return;
        }
        else {
            std::cout << "Created base directory: " << baseDir << std::endl;
        }
    }

    // 1_empty.txt ~ 5_empty.txt 파일 생성
    for (int i = 1; i <= 5; ++i) {
        std::string filePath = baseDir + "\\" + std::to_string(i) + "_empty.txt";

        if (!fileExists(filePath)) {
            std::ofstream outFile(filePath);
            if (outFile) {
                std::cout << "Created file: " << filePath << std::endl;
                outFile.close();
            }
            else {
                std::cerr << "Failed to create file: " << filePath << std::endl;
            }
        }
        else {
            std::cout << "File already exists: " << filePath << std::endl;
        }
    }
}
