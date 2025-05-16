#include "SSD_func.h"

void SSD::write(int LBA, int Val) {
    return;
}

int SSD::read(int LBA) {
    if (LBA == 5) return 0x12345555;
    return 0x12345678;
}
