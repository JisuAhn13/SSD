#pragma once
#include <iostream>
#include <windows.h>  // CreateDirectoryA ���
#include <string>
#include <fstream>
#include <vector>
#include "SSD_func.h"

struct command {       // ������ ��ɾ ��Ÿ���� ����ü
    char op;               // ������
    uint firstData;
    uint secondData;
};

class CommandBuffer {
public:
    enum {
        MAX_BUFFER_SIZE = 5,
    };

    CommandBuffer();

    inline bool full() const {
        return (buffer.size() >= CommandBuffer::MAX_BUFFER_SIZE);
    }

    void enqueue(command command);
    void flush();

private:
    // ���丮 ���� ���� Ȯ��
    bool directoryExists(const std::string& path);
    bool fileExists(const std::string& path);

    std::vector<command> buffer;  // ���� ��ɾ �����ϴ� ����
    SSD ssd;
};
