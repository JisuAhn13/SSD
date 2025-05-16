#pragma once
#include <fstream>
#include <string>

class SSD {
public:
    void write(int LBA, int Val);
    void initwrite();
    int read(int LBA);
};