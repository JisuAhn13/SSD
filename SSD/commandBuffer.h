#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_set>
#include "SSD_func.h"

enum {
    CMD_WRITE = 'W',
    CMD_READ = 'R',
    CMD_ERASE = 'E',
    CMD_FLUSH = 'F',
};

struct BufferCommand {
    char op;
    uint firstData;
    uint secondData;
};

class CommandBuffer {
public:
    enum {
        MAX_BUFFER_SIZE = 5,
    };

    // singleton structure
    static CommandBuffer& getInstance() {
        static CommandBuffer instance;
        return instance;
    }

    BufferCommand getBufferIndex(int i);

    bool fillCommandBufferWithFileNames(void);
    bool createDirectory(std::string& baseDir);
    void initializeCommandBuffer();

    unsigned int enqueue(BufferCommand cmd);
    void flush();
    void pushCMD(const BufferCommand cmd);
    void clearVec();
    void clearDir();
    void fileWrite();
    void eraseAlgorithm();
    void mergeAlgorithm();
    std::vector<std::string> getFileNamesInDirectory();

    inline bool isFull() const {
        return (buffer.size() >= CommandBuffer::MAX_BUFFER_SIZE);
    }

private:
    bool directoryExists(const std::string& path);
    bool fileExists(const std::string& path);
    bool bufferFileExists(const std::string& fileName);
    std::string removeTxt(std::string& token);
    BufferCommand getCommandFromFile(std::string fileName);
    void makeEmptyFiles(std::string& baseDir);
    bool readinbuffer(unsigned int lba, unsigned int& value);
    void optimizeCMD();

    std::vector<BufferCommand> buffer;
    SSD ssd;
};
