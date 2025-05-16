#include "SSD_func.h"

void SSD::write(unsigned int LBA, unsigned int Val) {
    const std::string filename = "ssd_nand.txt";

    std::ifstream checkfile(filename);
    if (!checkfile.good()) {
        checkfile.close();
        initwrite();
    }
    else {
        checkfile.close();
    }

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

    // 5. 수정된 줄 전체 다시 써주기
    std::ofstream outfile(filename);
    for (const auto& l : lines) {
        outfile << l << "\n";
    }
    outfile.close();
}

void SSD::initwrite() {
    std::ofstream ofs("ssd_nand.txt");
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

int SSD::read(int LBA) {
    return 0;
}