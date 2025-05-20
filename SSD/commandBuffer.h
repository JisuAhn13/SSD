#pragma once
#include <iostream>
#include <windows.h>  // CreateDirectoryA 사용
#include <string>
#include <fstream>
#include <vector>
#include "SSD_func.h"

enum {
    CMD_WRITE = 'W',
    CMD_READ = 'R',
    CMD_ERASE = 'E',
};

struct command {       // 실행할 명령어를 나타내는 구조체
    char op;               // 연산자
    uint firstData;
    uint secondData;
};

class CommandBuffer {
public:
    enum {
        MAX_BUFFER_SIZE = 5,
    };

    CommandBuffer();
    inline bool isFull() const {
        return (buffer.size() >= CommandBuffer::MAX_BUFFER_SIZE);
    }

    unsigned int enqueue(command cmd);
    void flush();
    bool readinbuffer(unsigned int lba, unsigned int& value);

    void pushCMD(const command cmd);
    void clearVec();
    void fileWrite();
    void clearDir();
    std::vector<std::string> getFileNamesInDirectory();

private:
    // 디렉토리 존재 여부 확인
    bool directoryExists(const std::string& path);
    bool fileExists(const std::string& path);
    std::vector<command> buffer;  // 여러 명령어를 저장하는 벡터
    SSD ssd;
};
