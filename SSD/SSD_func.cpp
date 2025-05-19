#include "SSD_func.h"

SSD::SSD() {
    std::string filename = this->getFileName();

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

std::string SSD::getFileName() {
    return this->filename;
}

void SSD::write(unsigned int LBA, unsigned int Val) {
    const std::string filename = this->getFileName();

    std::ifstream infile(filename);
    if (!infile.is_open()) return;

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();

    std::ostringstream newline;
    newline << std::setw(2) << std::setfill('0') << std::dec << LBA - 1
        << " 0x" << std::setw(8) << std::setfill('0') << std::uppercase <<std::hex << Val;
    lines[LBA - 1] = newline.str();

    std::ofstream outfile(filename);
    for (const auto& l : lines) {
        outfile << l << "\n";
    }
    outfile.close();
}

int SSD::read(int LBA) {
    return 0;
}