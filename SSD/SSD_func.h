#pragma once
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
class SSD {
public:
    SSD();
    std::string getFileName();
    void write(unsigned int LBA, unsigned int Val);
    int read(int LBA);
private:
    const std::string filename = "ssd_nand.txt";
};