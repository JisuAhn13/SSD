#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <regex>
typedef unsigned int uint;

class SSD {
public:
    SSD();
    std::string getDataFileName();
    std::string getOuputFileName();
    void write(uint LBA, uint Val);
    uint read(uint LBA);
    void erase(uint StartLBA, uint EndLBA);
    uint readDataFromLBA(std::ifstream& inFile, const uint& LBA);
    void recordFile(uint LBA, uint data);

private:
    const std::string outputFile = "ssd_output.txt";
    const std::string DataFile = "ssd_nand.txt";
};