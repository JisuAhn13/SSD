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

void CommandBuffer::pushCMD(const command cmd) {
    this->buffer.push_back(cmd);
}

void CommandBuffer::clearVec() {
    this->buffer.clear();
}

void CommandBuffer::fileWrite() {
    std::string baseDir = "buffer";
    std::string filePath;

    this->clearDir();

    for (int idx = 1; idx <= this->buffer.size(); idx++) {
        filePath = baseDir + "\\" + std::to_string(idx) +"_"+ this->buffer[idx - 1].op + "_"
            + std::to_string(this->buffer[idx-1].firstData) + "_";
        
        if (this->buffer[idx-1].op == 'E') {
            filePath += std::to_string(this->buffer[idx-1].secondData);
        }
        else {
            std::ostringstream oss;
            oss << std::uppercase
                << std::hex
                << this->buffer[idx-1].secondData;

            filePath += oss.str();
        }
        filePath += ".txt";

        if (!fileExists(filePath)) {
            std::ofstream outFile(filePath);
            if (outFile) {
                outFile.close();
            }
        }
    }

    for (int idx = this->buffer.size() + 1; idx <= 5; idx++) {
        filePath = baseDir + "\\" + std::to_string(idx) + "_empty.txt";

        if (!fileExists(filePath)) {
            std::ofstream outFile(filePath);
            if (outFile) {
                outFile.close();
            }
        }
    }
}

void CommandBuffer::clearDir() {
    std::string searchPath = "buffer\\*";

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        std::string fileName = findData.cFileName;

        if (fileName == "." || fileName == "..") continue;

        std::string fullPath = "buffer\\" + fileName;

        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            DeleteFileA(fullPath.c_str());
        }

    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
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