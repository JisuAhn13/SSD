#pragma once
#include <iostream>
#include <windows.h>  // CreateDirectoryA 사용
#include <string>
#include <fstream>

class CommandBuffer {
public:
    CommandBuffer();

private:
    // 디렉토리 존재 여부 확인
    bool directoryExists(const std::string& path);
    bool fileExists(const std::string& path);
};
