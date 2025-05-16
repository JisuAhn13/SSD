#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define UNIT_TEST_WITHOUT_WRITE
typedef unsigned int uint;

class SSD {
public:
    void write(uint LBA, uint Val);
    uint read(uint LBA);
};