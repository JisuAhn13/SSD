#include "SSD_func.h"

using namespace std;

/*
* Function to remove the "0x" prefix from hex strings
*/
void removeHexPrefix(std::string& data)
{
    if (data.rfind("0x", 0) == 0) {
        data = data.substr(2);
    }
}


void validateFileOpen(std::ifstream& inFile)
{
    if (!inFile) {
        std::cerr << "can't open file" << std::endl;
        throw(std::exception());
    }
}

/*
* Find LBA in File and return data
*/
uint readDataFromLBA(std::ifstream& inFile, const uint& LBA)
{
    std::string line, data;
    int readLBA;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        if (iss >> readLBA >> data && readLBA == LBA) {
            removeHexPrefix(data);
            return std::stoi(data, nullptr, 16);
        }
    }
    std::cerr << "search Fail" << std::endl;
    std::exception();
}


void SSD::write(uint LBA, uint Val) {
    return;
}

uint SSD::read(uint LBA) {
#ifdef UNIT_TEST_WITHOUT_WRITE
    std::ifstream inFile("testLBA.txt");
#else
    std::ifstream inFile("ssd_nand.txt");
#endif 

    validateFileOpen(inFile);

    return readDataFromLBA(inFile, LBA);
}
