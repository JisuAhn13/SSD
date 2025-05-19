#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

#define UNIT_TEST_WITHOUT_WRITE
typedef unsigned int uint;

class SSD {
public:
    SSD();
    std::string getFileName();
    void write(uint LBA, uint Val);
    uint read(uint LBA);
    uint readDataFromLBA(std::ifstream& inFile, const uint& LBA);
    std::string getOuputFileName();
    void recordFile(uint LBA, uint data);

private:
    const std::string outputFile = "ssd_output.txt";
    const std::string filename = "ssd_nand.txt";
};