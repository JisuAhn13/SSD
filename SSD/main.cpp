#include "gmock/gmock.h"
#include "SSD_func.h"
#include "CommandChecker.h"
#include "commandBuffer.h"

using namespace testing;

int main(int argc, char* argv[]) {

    CommandBuffer  cmd;
#ifdef _DEBUG
    InitGoogleMock();
    return RUN_ALL_TESTS();
#endif
    CommandChecker commandchecker;
    commandchecker.execute(argc, argv);
    return 0;
}
