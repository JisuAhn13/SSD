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

bool CommandBuffer::readinbuffer(unsigned int lba, unsigned int& value)
{
    for (auto iter = buffer.rbegin(); iter != buffer.rend(); ++iter) {
        command cmd = *iter;
        if (cmd.op == CMD_WRITE) {
            if (cmd.firstData == lba) {
                value = cmd.secondData;
                return true;
            }
        }

        if (cmd.op == CMD_ERASE) {
            if (cmd.firstData >= lba && cmd.secondData <= lba) {
                value = 0x0;
                return true;
            }
        }
    }
}

// Call from CommandChecker
unsigned int CommandBuffer::enqueue(command cmd)
{
    // 0. Import buffer files (if not exist, create files)

    // 1-1. If buffer is full(size:5), execute all commands
    if (full()) {
        flush();
    }

    // 1-2. Enqueue command to buffer
    unsigned int value = 0;
    if (cmd.op == 'W' || cmd.op == 'E') {
        buffer.push_back(cmd);
    }
    else if (cmd.op == 'R') {
        bool ret = readinbuffer(cmd.firstData, value);
        if (ret == false) {
            // if data can be decided without reading ssd_nand.txt, then ssd read
            value = ssd.read(cmd.firstData);
        }
    }
    else {
        // Invalid Operator
    }

    // 2. Optimize
    bool ret = readinbuffer(cmd.firstData, value);
    if (ret == false) {
        // if data can be decided without reading ssd_nand.txt, then ssd read
        value = ssd.read(cmd.firstData);
    }

    // 3. Export buffer files

    return value;
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