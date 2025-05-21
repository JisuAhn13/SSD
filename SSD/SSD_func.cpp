#include "SSD_func.h"
using namespace std;

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

void checkLBAValidity(uint LBA) {
    if (LBA < 0 || LBA > 99) {
        std::cerr << "invalide LBA Address Fail" << std::endl;
        throw std::exception();
    }
}

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
            return std::stoll(data, nullptr, 16);
        }
    }
    std::cerr << "search Fail" << std::endl;
    throw std::exception();
}

std::string SSD::getOuputFileName() {
    return this->outputFile;
}

void SSD::recordFile(uint LBA, uint data) {
    std::ofstream outfile(getOuputFileName(), std::ios::trunc);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file for writing.\n";
        return;
    }

    outfile << "0x"
        << std::uppercase << std::setfill('0') << std::setw(8)
        << std::hex << data << std::endl;

    outfile.close();
}

uint SSD::read(uint LBA) {
    std::ifstream inFile(this->getDataFileName());
    checkLBAValidity(LBA);

    uint readData = readDataFromLBA(inFile, LBA);
    inFile.close();

    recordFile(LBA, readData);
    return readData;
}

SSD::SSD() {
    std::string filename = this->getDataFileName();

    std::ifstream checkFile(filename);
    if (checkFile.good()) return;
    checkFile.close();

    std::ofstream ofs(filename);
    if (!ofs.is_open()) return;

    for (int i = 0; i < 100; ++i) {
        std::ostringstream line;
        line << std::setw(2) << std::setfill('0') << i;
        line << " 0x" << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << 0;
        ofs << line.str() << "\n";
    }

    ofs.close();
    return;
}

std::string SSD::getDataFileName() {
    return this->DataFile;
}

void SSD::write(unsigned int LBA, unsigned int Val) {
    const std::string filename = this->getDataFileName();

    std::ifstream infile(filename);
    if (!infile.is_open()) return;

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();

    std::ostringstream newline;
    newline << std::setw(2) << std::setfill('0') << std::dec << LBA
        << " 0x" << std::setw(8) << std::setfill('0') << std::uppercase <<std::hex << Val;
    lines[LBA] = newline.str();

    std::ofstream outfile(filename);
    for (const auto& l : lines) {
        outfile << l << "\n";
    }
    outfile.close();
}

void SSD::erase(uint StartLBA, uint Size) {
    for (int idx = 0; idx < Size; idx++) {
        this->write(StartLBA+idx, 0x00000000);
    }
}