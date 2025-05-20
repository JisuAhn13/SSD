#pragma once
#include <iostream>
#include <windows.h>  // CreateDirectoryA 사용
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
    // 디렉토리 존재 여부 확인
    bool directoryExists(const std::string& path);
    bool fileExists(const std::string& path);
    std::vector<Command> buffer;  // 여러 명령어를 저장하는 벡터
};

