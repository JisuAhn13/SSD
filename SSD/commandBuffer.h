#pragma once
#include <iostream>
#include <windows.h>  // CreateDirectoryA ���
#include <string>
#include <fstream>
#include <vector>

struct command {
    char op;
    unsigned int firstData;
    unsigned int secondData;
};

class CommandBuffer {
public:
    CommandBuffer();
    std::vector<std::string> getFileNamesInDirectory();

private:
    // ���丮 ���� ���� Ȯ��
    bool directoryExists(const std::string& path);
    bool fileExists(const std::string& path);
    std::vector<command> buffer;  // ���� ��ɾ �����ϴ� ����
};

void removeTxt(std::string& token);
