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
    CommandBuffer();
    void pushCMD(const command cmd);
    void clearVec();
    void fileWrite();
    void clearDir();
    void eraseAlgorithm();
    void mergeAlgorithm();
    void optimizeCMD();
    std::vector<std::string> getFileNamesInDirectory();
private:
    // ���丮 ���� ���� Ȯ��
    bool directoryExists(const std::string& path);
    bool fileExists(const std::string& path);
    std::vector<command> buffer;  // ���� ��ɾ �����ϴ� ����
};