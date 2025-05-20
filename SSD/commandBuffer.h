#pragma once
#include <iostream>
#include <windows.h>  // CreateDirectoryA ���
#include <string>
#include <fstream>
#include <vector>

struct Command {
    char op;
    unsigned int firstData;
    unsigned int secondData;
};

class CommandBuffer {
public:
    CommandBuffer();
    Command getCommandFromFile(std::string fileName);

    void removeTxt(std::string& token);
    std::vector<std::string> getFileNamesInDirectory();
    Command getBufferIndex(int i);
private:
    // ���丮 ���� ���� Ȯ��
    bool directoryExists(const std::string& path);
    bool fileExists(const std::string& path);
    std::vector<Command> buffer;  // ���� ��ɾ �����ϴ� ����
};

