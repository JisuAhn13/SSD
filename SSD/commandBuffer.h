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
    void createBufferDirectory(std::string& baseDir);
    void createEmptyFiles(std::string& baseDir);
    CommandBuffer();
    bool initializeBufferToFileLists();
    command getCommandFromFile(std::string fileName);

    void removeTxt(std::string& token);
    std::vector<std::string> getFileNamesInDirectory();
    command getBufferIndex(int i);
private:
    // ���丮 ���� ���� Ȯ��
    bool directoryExists(const std::string& path);
    bool fileExists(const std::string& path);
    std::vector<command> buffer;  // ���� ��ɾ �����ϴ� ����
};

