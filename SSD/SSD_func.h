#pragma once
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>

class SSD {
public:
    void write(unsigned int LBA, unsigned int Val);
    void initwrite();
    int read(int LBA);
};