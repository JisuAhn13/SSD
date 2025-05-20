#include "commandBuffer.h"
#include "SSD_func.h"

bool CommandBuffer::directoryExists(const std::string& path) {
    DWORD ftyp = GetFileAttributesA(path.c_str());
    return (ftyp != INVALID_FILE_ATTRIBUTES && (ftyp & FILE_ATTRIBUTE_DIRECTORY));
}

bool CommandBuffer::fileExists(const std::string& path) {
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}


void CommandBuffer::removeTxt(std::string& token)
{
    size_t pos = token.find(".txt");
    if (pos != std::string::npos) {
        token = token.substr(0, pos);
    }
}

command  CommandBuffer::getCommandFromFile(std::string fileName) {
    std::stringstream ss(fileName);
    std::string token;
    command ret = { 0, };
    while (std::getline(ss, token, '_')) {

        if (token == "empty.txt") {
            removeTxt(token);
            return ret;
        }

        if (token == "W" || token == "E") {
            ret.op = token[0];
        }
        else if (ret.firstData == 0 && token.length() == 2) {
            ret.firstData = stoul(token);
        }
        else if (token.length() >= 2) {
            removeTxt(token);
            ret.secondData = std::stoul(token, nullptr, 16);
        }
    }
    return ret;
}

bool bufferFileExists(const std::string& fileName) {
    std::regex pattern(R"([1-5]_.*\.txt)");
    return std::regex_match(fileName, pattern);
}

command CommandBuffer::getBufferIndex(int i) {
    return CommandBuffer::buffer[i];
}

void CommandBuffer::makeEmptyFiles(std::string& baseDir)
{
    for (int i = 1; i <= 5; ++i) {
        std::string filePath = baseDir + "\\" + std::to_string(i) + "_empty.txt";

        if (!fileExists(filePath)) {
            std::ofstream outFile(filePath);
            if (outFile) {
                outFile.close();
            }
        }
    }
}

bool CommandBuffer::fillCommandBufferWithFileNames()
{
    std::vector<std::string> bufferFileLists = getFileNamesInDirectory();

    bool fileChecker = false;
    for (const auto& fileName : bufferFileLists) {
        if (bufferFileExists(fileName)) {
            command cmd = getCommandFromFile(fileName);
            buffer.push_back(cmd);
            fileChecker = true;
        }
    }
    return fileChecker;
}

bool CommandBuffer::createDirectory(std::string& baseDir)
{
    if (!directoryExists(baseDir)) {
        if (!CreateDirectoryA(baseDir.c_str(), NULL)) {
            return true;
        }
    }
    return false;
}

CommandBuffer::CommandBuffer() {
    std::string baseDir = "buffer";
    
    if (createDirectory(baseDir)) return;

    if (fillCommandBufferWithFileNames()) return;

    // make 1_empty.txt ~ 5_empty.txt files
    makeEmptyFiles(baseDir);
}

bool CommandBuffer::readinbuffer(unsigned int lba, unsigned int& value)
{
    for (auto iter = buffer.rbegin(); iter != buffer.rend(); ++iter) {
        command cmd = *iter;
        if (cmd.op == CMD_WRITE) {
            if (cmd.firstData == lba) {
                value = cmd.secondData;
                return true;
            }
        }

        if (cmd.op == CMD_ERASE) {
            if (cmd.firstData >= lba && cmd.secondData <= lba) {
                value = 0x0;
                return true;
            }
        }
    }
    return true;
}

// Call from CommandChecker
unsigned int CommandBuffer::enqueue(command cmd)
{
    // 0. Import buffer files (if not exist, create files)

    // 1-1. If buffer is full(size:5), execute all commands
    if (isFull()) {
        flush();
    }

    // 1-2. Enqueue command to buffer
    unsigned int value = 0;
    if (cmd.op == 'W' || cmd.op == 'E') {
        buffer.push_back(cmd);
    }
    else if (cmd.op == 'R') {
        bool ret = readinbuffer(cmd.firstData, value);
        if (ret == false) {
            // if data can be decided without reading ssd_nand.txt, then ssd read
            value = ssd.read(cmd.firstData);
        }
    }
    else {
        // Invalid Operator
    }

    // 2. Optimize

    // 3. Export buffer files

    return value;
}

void CommandBuffer::flush() {
    for (auto cmd : buffer) {
        if (cmd.op == 'W') {
            ssd.write(cmd.firstData, cmd.secondData);
        }
        else if (cmd.op == 'E') {
            ssd.erase(cmd.firstData, cmd.secondData);
        }
        else {
            std::exception();
        }
    }
}

void CommandBuffer::pushCMD(const command cmd) {
    this->buffer.push_back(cmd);
}

void CommandBuffer::clearVec() {
    this->buffer.clear();
}

void CommandBuffer::fileWrite() {
    std::string baseDir = "buffer";
    std::string filePath;

    this->clearDir();

    for (int idx = 1; idx <= this->buffer.size(); idx++) {
        filePath = baseDir + "\\" + std::to_string(idx) +"_"+ this->buffer[idx - 1].op + "_"
            + std::to_string(this->buffer[idx-1].firstData) + "_";
        
        if (this->buffer[idx-1].op == 'E') {
            filePath += std::to_string(this->buffer[idx-1].secondData);
        }
        else {
            std::ostringstream oss;
            oss << std::uppercase
                << std::hex
                << this->buffer[idx-1].secondData;

            filePath += oss.str();
        }
        filePath += ".txt";

        if (!fileExists(filePath)) {
            std::ofstream outFile(filePath);
            if (outFile) {
                outFile.close();
            }
        }
    }

    for (int idx = (int)this->buffer.size() + 1; idx <= 5; idx++) {
        filePath = baseDir + "\\" + std::to_string(idx) + "_empty.txt";

        if (!fileExists(filePath)) {
            std::ofstream outFile(filePath);
            if (outFile) {
                outFile.close();
            }
            else {
                std::exception();
            }
        }
    }
}

void CommandBuffer::clearDir() {
    std::string searchPath = "buffer\\*";

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        std::string fileName = findData.cFileName;

        if (fileName == "." || fileName == "..") continue;

        std::string fullPath = "buffer\\" + fileName;

        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            DeleteFileA(fullPath.c_str());
        }

    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
}

void CommandBuffer::eraseAlgorithm() {

}

void CommandBuffer::mergeAlgorithm() {

}

void CommandBuffer::optimizeCMD() {
    this->eraseAlgorithm();
    this->mergeAlgorithm();
}


std::vector<std::string> CommandBuffer::getFileNamesInDirectory() {
    std::vector<std::string> fileNames;
    std::string directoryPath = "buffer";
    std::string searchPath = directoryPath + "\\*";

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return fileNames;
    }

    do {
        std::string name = findData.cFileName;

        if (name != "." && name != "..") {
            fileNames.push_back(name);
        }
    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
    return fileNames;
}
