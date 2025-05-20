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

// Call from CommandChecker
void CommandBuffer::enqueue(command command)
{
    // 0. Import buffer files (if not exist, create files)

    // 1-1. If buffer is full(size:5), execute all commands
    if (full()) {
        flush();
    }

    // 1-2. Enqueue command to buffer
    if (command.op == 'W' || command.op == 'E') {
        buffer.push_back(command);
    }
    else if (command.op == 'R') {
        // check if data can be decided without reading ssd_nand.txt
        ssd.read(command.firstData);
    }

    // 2. Optimize

    // 3. Export buffer files
}

void CommandBuffer::flush() {
    for (auto c : buffer) {
        if (c.op == 'W') {
            ssd.write(c.firstData, c.secondData);
        }
        else if (c.op == 'E') {
            ssd.erase(c.firstData, c.secondData);
        }
        else {
            std::cout << "Invalid Command" << std::endl;
        }
    }
}