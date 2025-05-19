#include "gmock/gmock.h"
#include "SSD_func.h"
#include "CommandChecker.h"
using namespace testing;

int main(int argc, char* argv[]) {
    
    CommandChecker commandchecker;
    commandchecker.execute(argc, argv);
    

    return 0;
    //InitGoogleMock();
    //return RUN_ALL_TESTS();
}
