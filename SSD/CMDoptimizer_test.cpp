#include "gmock/gmock.h"
#include "commandBuffer.h"
#include <windows.h>
#include <algorithm>
using namespace testing;

class CmdBufferFixture : public ::testing::Test {
protected:

    void feelCmdBuffer(std::vector<BufferCommand> TC_Input) {
        cmdBuf.clearVec();
        cmdBuf.copyBuffer(TC_Input);
    }

    bool compareCMD(BufferCommand cmd1, BufferCommand cmd2) {
        if (cmd1.op != cmd2.op) {
            return false;
        }
        if (cmd1.firstData != cmd2.firstData) {
            return false;
        }
        if (cmd1.secondData != cmd2.secondData) {
            return false;
        }

        return true;
    }

    std::vector<BufferCommand> loadCommands(const std::string& filePath) {
        std::ifstream in(filePath);
        std::vector<BufferCommand> cmds;
        std::string op;
        unsigned int first, second;
        while (in >> op >> first >> std::ws) {
            if (op == "W") {
                std::string hexVal;
                in >> hexVal;
                second = std::stoul(hexVal, nullptr, 16);
            }
            else {
                in >> second;
            }
            cmds.push_back({ op[0], first, second });
        }
        return cmds;
    }

    bool compareBuffer(std::vector<BufferCommand>TC_Output) {
        if (cmdBuf.getBufSize() != TC_Output.size()) {
            return false;
        }

        for (int idx = 0; idx < cmdBuf.getBufSize(); idx++) {
            if (!compareCMD(cmdBuf.getBufferIndex(idx),TC_Output[idx])) {
                return false;
            }
        }

        return true;
    }

    CommandBuffer& cmdBuf = CommandBuffer::getInstance();
};

TEST_F(CmdBufferFixture, ParamTestFromFile) {
    for (int i = 1; i <= 25; ++i) {
        std::string idx = (i < 10 ? "0" : "") + std::to_string(i);
        std::string inputFile = "testcases/TC" + idx + "_input.txt";
        std::string expectFile = "testcases/TC" + idx + "_expected.txt";

        std::vector<BufferCommand> tcInput = loadCommands(inputFile);
        std::vector<BufferCommand> tcOutput = loadCommands(expectFile);

        feelCmdBuffer(tcInput);
        cmdBuf.eraseAlgorithm();
        cmdBuf.mergeAlgorithm();
       
        ASSERT_TRUE(compareBuffer(tcOutput))<<"Test Fail : "<<i;
    }
}