#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <exception>
#include "SSD_func.h"
#include <set>
#include <algorithm>

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

class CommandBufferException : public std::exception {
public:
    explicit CommandBufferException(const std::string& message)
        : msg_(message) {}

    virtual const char* what() const noexcept override {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

class CommandBuffer {
public:
    enum {
        MAX_BUFFER_SIZE = 5,
    };

    CommandBuffer();
    ~CommandBuffer();

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
    void copyBuffer(std::vector<BufferCommand> buf);
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
    void createEmptyFilesForRemaining(std::string& baseDir);
    BufferCommand getCommandFromFile(std::string fileName);
    bool fastRead(unsigned int lba, unsigned int& value);
    void optimizeCMD();

    std::vector<BufferCommand> buffer;
    SSD ssd;
};
