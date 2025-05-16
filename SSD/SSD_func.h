#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class SSD {
public:
    void write(int LBA, int Val);
    int read(int LBA);
};