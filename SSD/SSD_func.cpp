#include "SSD_func.h"

#define UNIT_TEST_WITHOUT_WRITE

void SSD::write(int LBA, int Val) {
    return;
}

int SSD::read(int LBA) {
#ifdef UNIT_TEST_WITHOUT_WRITE
    std::ifstream inFile("testLBA.txt");
#else
    std::ifstream inFile("ssd_nand.txt");
#endif 

    if (!inFile) {
        std::cerr << "can't open file" << std::endl;
        throw(std::exception());
    }

    std::string line;
    int readLBA;
    std::string data;
    int parsedVal=0;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        if (iss >> readLBA >> data && readLBA == LBA) {
            if (data.rfind("0x", 0) == 0) {
                data = data.substr(2);
            }
            parsedVal = std::stoi(data, nullptr, 16);
        }
    }
    return parsedVal;
}