#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <vector>
#include "SSD_func.h"

enum {
    CMD_WRITE = 'W',
    CMD_READ = 'R',
    CMD_ERASE = 'E',
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

    void makeEmptyFiles(std::string& baseDir);

    bool fillCommandBufferWithFileNames(void);

    bool createDirectory(std::string& baseDir);

    CommandBuffer();
    inline bool isFull() const {
        return (buffer.size() >= CommandBuffer::MAX_BUFFER_SIZE);
    }

    unsigned int enqueue(BufferCommand cmd);
    void flush();
    bool readinbuffer(unsigned int lba, unsigned int& value);

    void pushCMD(const BufferCommand cmd);
    void clearVec();
    void fileWrite();
    void clearDir();
    void eraseAlgorithm();
    void mergeAlgorithm();
    void optimizeCMD();
    void removeTxt(std::string& token);

    BufferCommand getCommandFromFile(std::string fileName);
    std::vector<std::string> getFileNamesInDirectory();

    BufferCommand getBufferIndex(int i);

private:
    bool directoryExists(const std::string& path);
    bool fileExists(const std::string& path);
    std::vector<BufferCommand> buffer;
    SSD ssd;
};
