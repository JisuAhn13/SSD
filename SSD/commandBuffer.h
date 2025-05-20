#pragma once
#include <iostream>
#include <windows.h>  // CreateDirectoryA ���
#include <string>
#include <fstream>

class CommandBuffer {
public:
    CommandBuffer();

private:
    // ���丮 ���� ���� Ȯ��
    bool directoryExists(const std::string& path);
    bool fileExists(const std::string& path);
};
