#include "commandBuffer.h"
#include "SSD_func.h"

// ���丮 ���� ���� Ȯ��
bool CommandBuffer::directoryExists(const std::string& path) {
    DWORD ftyp = GetFileAttributesA(path.c_str());
    return (ftyp != INVALID_FILE_ATTRIBUTES && (ftyp & FILE_ATTRIBUTE_DIRECTORY));
}

bool CommandBuffer::fileExists(const std::string& path) {
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}

// ������ 
CommandBuffer::CommandBuffer() {
    std::string baseDir = "buffer";

    // buffer ���� ����
    if (!directoryExists(baseDir)) {
        if (!CreateDirectoryA(baseDir.c_str(), NULL)) {
            std::cerr << "Failed to create base directory: " << baseDir << std::endl;
            return;
        }
        else {
            std::cout << "Created base directory: " << baseDir << std::endl;
        }
    }

    // 1_empty.txt ~ 5_empty.txt ���� ����
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

void CommandBuffer::pushCMD(const command cmd) {
    this->buffer.push_back(cmd);
}

void CommandBuffer::clearVec() {
    this->buffer.clear();
}

void CommandBuffer::fileWrite() {

}

std::vector<std::string> CommandBuffer::getFileNamesInDirectory() {
    std::vector<std::string> fileNames;
    std::string directoryPath = "buffer";
    std::string searchPath = directoryPath + "\\*";

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return fileNames;
    }

    do {
        std::string name = findData.cFileName;

        if (name != "." && name != "..") {
            fileNames.push_back(name);
        }
    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
    return fileNames;
}