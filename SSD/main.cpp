#include "gmock/gmock.h"
#include "SSD_func.h"
#include "CommandChecker.h"
#include "commandBuffer.h"

using namespace testing;

int main(int argc, char* argv[]) {

#ifdef _DEBUG
    InitGoogleMock();
    return RUN_ALL_TESTS();
#endif
    CommandBuffer  cmd;
    CommandChecker commandchecker;
    commandchecker.execute(argc, argv);
    

    return 0;
}
