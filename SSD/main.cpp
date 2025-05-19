#include "gmock/gmock.h"
#include "SSD_func.h"
#include "CommandChecker.h"
using namespace testing;
#define UNIT_TEST
int main(int argc, char* argv[]) {

#ifdef UNIT_TEST
    InitGoogleMock();
    return RUN_ALL_TESTS();
#endif
    CommandChecker commandchecker;
    commandchecker.execute(argc, argv);
    

    return 0;
}
