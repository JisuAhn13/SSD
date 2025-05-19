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
#ifdef UNIT_TEST_WITHOUT_WRITE
        std::cerr << "check your testLBA.txt file is valid" << std::endl;
#endif
        throw(std::exception());
    }
}

void checkLBAValidity(uint LBA) {
    if (LBA < 0 || LBA > 99) {
        std::cerr << "invalide LBA Address Fail" << std::endl;
        throw std::exception();
    }
}
/*
* Find LBA in File and return data
*/
uint SSD::readDataFromLBA(std::ifstream& inFile, const uint& LBA)
{
    validateFileOpen(inFile);

    std::string line, data;
    int readLBA;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        if (iss >> readLBA >> data && readLBA == LBA) {
            removeHexPrefix(data);
            if (data.empty())return 0;
            return std::stol(data, nullptr, 16);
        }
    }
    std::cerr << "search Fail" << std::endl;
    throw std::exception();
}

std::string SSD::getOuputFileName() {
    return this->outputFile;
}

void SSD::recordFile(uint LBA, uint data) {
    // overwrite 
    std::ofstream outfile(getOuputFileName(), std::ios::trunc);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file for writing.\n";
        return;
    }

    // outfile : LBA 0x + Data
    outfile << "0x"
        << std::uppercase << std::setfill('0') << std::setw(8)
        << std::hex << data << std::endl;

    outfile.close();
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
    checkLBAValidity(LBA);

    uint readData = readDataFromLBA(inFile, LBA);
    inFile.close();

    recordFile(LBA, readData);
    return readData;
}
