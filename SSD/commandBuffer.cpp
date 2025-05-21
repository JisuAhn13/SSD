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

bool CommandBuffer::bufferFileExists(const std::string& fileName) {
	std::regex pattern(R"([1-5]_.*\.txt)");
	return std::regex_match(fileName, pattern);
}

std::string CommandBuffer::removeTxt(std::string& filename)
{

	size_t pos = filename.find(".txt");
	if (pos != std::string::npos)
		return filename.substr(0, pos);
	return filename;
}

BufferCommand  CommandBuffer::getCommandFromFile(std::string fileName) {
	std::stringstream ss(fileName);
	std::string token;
	BufferCommand ret = {};

	int tokenIndex = 0;
	while (std::getline(ss, token, '_')) {
		token = removeTxt(token);
		if (token == "empty") {
			return ret;
		}

		switch (tokenIndex) {
		case 1:
			if (token == "W" || token == "E") {
				ret.op = token[0];
			}
			break;
		case 2:
			ret.firstData = static_cast<uint32_t>(std::stoul(token));
			break;
		case 3:
			ret.secondData = static_cast<uint32_t>(std::stoul(token, nullptr, 16));
			break;
		}
		tokenIndex++;
	}
	return ret;
}

BufferCommand CommandBuffer::getBufferIndex(int i) {
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
			BufferCommand cmd = getCommandFromFile(fileName);
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

void CommandBuffer::initializeCommandBuffer() {
	std::string baseDir = "buffer";
	if (createDirectory(baseDir)) return;
	if (fillCommandBufferWithFileNames()) return;
	makeEmptyFiles(baseDir);
}

bool CommandBuffer::readinbuffer(unsigned int lba, unsigned int& value)
{
	for (auto iter = buffer.rbegin(); iter != buffer.rend(); ++iter) {
		BufferCommand cmd = *iter;
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
unsigned int CommandBuffer::enqueue(BufferCommand cmd)
{
	// 0. Import buffer files (if not exist, create files)
	initializeCommandBuffer();

	// 1-1. If buffer is full(size:5), execute all commands
	if (cmd.op == 'F' || isFull()) {
		flush();
		if (cmd.op == 'F') return 0;
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
	optimizeCMD();

	// 3. Export buffer files
	fileWrite();

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

void CommandBuffer::pushCMD(const BufferCommand cmd) {
	this->buffer.push_back(cmd);
}

void CommandBuffer::clearVec() {
	this->buffer.clear();
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

void CommandBuffer::fileWrite() {
	std::string baseDir = "buffer";
	std::string filePath;

	this->clearDir();

	for (int idx = 1; idx <= this->buffer.size(); idx++) {
		filePath = baseDir + "\\" + std::to_string(idx) + "_" + this->buffer[idx - 1].op + "_"
			+ std::to_string(this->buffer[idx - 1].firstData) + "_";

		if (this->buffer[idx - 1].op == 'E') {
			filePath += std::to_string(this->buffer[idx - 1].secondData);
		}
		else {
			std::ostringstream oss;
			oss << std::uppercase
				<< std::hex
				<< this->buffer[idx - 1].secondData;

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

void CommandBuffer::eraseAlgorithm() {
	std::unordered_set<uint> affectedAddresses;
	std::vector<BufferCommand> result;

	for (auto it = buffer.rbegin(); it != buffer.rend(); ++it) {
		bool overlaps = false;

		if (it->op == 'E') {
			for (uint addr = it->firstData; addr <= it->secondData; ++addr) {
				if (affectedAddresses.count(addr)) {
					overlaps = true;
					break;
				}
			}

			if (!overlaps) {
				for (uint addr = it->firstData; addr <= it->secondData; ++addr) {
					affectedAddresses.insert(addr);
				}
				result.push_back(*it);
			}
		}
		else if (it->op == 'W') {
			if (!affectedAddresses.count(it->firstData)) {
				affectedAddresses.insert(it->firstData);
				result.push_back(*it);
			}
		}
	}

	std::reverse(result.begin(), result.end());
	buffer = std::move(result);
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

