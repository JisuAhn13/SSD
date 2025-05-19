#include "gmock/gmock.h"
#include <string>
//string은 최종 Merge시 삭제
#include "SSD_func.h"
#include "CommandChecker.h"
using namespace testing;

int main(int argc, char* argv[]) {
    
    bool isValidCommand;
    
    isValidCommand = execute(int argc, char* argv[]);
    
    if (isValidCommand) {   
        SSD ssd;

        if (argv[1] == "R") {
            ssd.read(std::stoi(std::string(argv[2])));
        }
        else {
            ssd.write(std::stoi(std::string(argv[2])), std::stoi(std::string(argv[3])));
        }
    }

    return 0;
    //InitGoogleMock();
    //return RUN_ALL_TESTS();
}
