#include "SSD_func.h"

void SSD::write(unsigned int LBA, unsigned int Val) {
    return;
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