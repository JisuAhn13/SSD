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

std::vector<unsigned int> loadInitialNand(const std::string& filename) {
	std::ifstream file(filename);
	std::vector<unsigned int> nand(100, 0);
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		int lba;
		std::string valHex;
		if (!(iss >> lba >> valHex)) continue;
		nand[lba] = std::stoul(valHex, nullptr, 16);
	}
	return nand;
}

void applyCommands(const std::vector<BufferCommand>& commands, std::vector<unsigned int>& nand) {
	for (const auto& cmd : commands) {
		if (cmd.op == 'W') {
			nand[cmd.firstData] = cmd.secondData;
		}
		else if (cmd.op == 'E') {
			for (unsigned int i = 0; i < cmd.secondData; ++i) {
				if (cmd.firstData + i < nand.size()) {
					nand[cmd.firstData + i] = 0;
				}
			}
		}
	}
}

bool nandEqual(const std::vector<unsigned int>& a, const std::vector<unsigned int>& b) {
	return a == b;
}

std::vector<std::pair<int, int>> collectErasableRanges(const std::vector<BufferCommand>& commands) {
	std::vector<std::pair<int, int>> ranges;
	for (const auto& cmd : commands) {
		if (cmd.op == 'E') {
			int s = cmd.firstData;
			int e = s + cmd.secondData - 1;
			ranges.emplace_back(s, e);
		}
	}
	std::sort(ranges.begin(), ranges.end());
	std::vector<std::pair<int, int>> merged;
	for (size_t i = 0; i < ranges.size(); ++i) {
		int s = ranges[i].first;
		int e = ranges[i].second;
		if (!merged.empty() && s <= merged.back().second + 1) {
			merged.back().second = std::max<int>(merged.back().second, e);
		}
		else {
			merged.emplace_back(s, e);
		}
	}
	std::vector<std::pair<int, int>> result;
	for (size_t i = 0; i < merged.size(); ++i) {
		int s = merged[i].first;
		int e = merged[i].second;
		if (e - s + 1 <= 10) {
			result.emplace_back(s, e);
		}
	}
	return result;
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
			// 예외 처리: 비어 있는 명령
			return ret;
		}

		switch (tokenIndex) {
		case 1: // W 또는 E
			if (token == "W" || token == "E") {
				ret.op = token[0];
			}
			break;
		case 2: // firstData
			ret.firstData = static_cast<uint32_t>(std::stoul(token));
			break;
		case 3: // secondData (16진수)
			ret.secondData = static_cast<uint32_t>(std::stoul(token, nullptr, 16));
			break;
		}
		tokenIndex++;
	}

	return ret;
}

bool bufferFileExists(const std::string& fileName) {
	std::regex pattern(R"([1-5]_.*\.txt)");
	return std::regex_match(fileName, pattern);
}

BufferCommand CommandBuffer::getBufferIndex(int i) {
	return CommandBuffer::buffer[i];
}

void CommandBuffer::copyBuffer(std::vector<BufferCommand> buf) {
	this->clearVec();

	for (int idx = 0; idx < buf.size(); idx++) {
		this->buffer.push_back(buf[idx]);
	}
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

void CommandBuffer::pushCMD(const BufferCommand cmd) {
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

void CommandBuffer::mergeAlgorithm()
{
	const std::vector<BufferCommand>& original = this->buffer;
	std::vector<std::pair<int, int>> mergedErasures = collectErasableRanges(original);
	std::vector<unsigned int> nand = loadInitialNand("ssd_nand.txt");
	applyCommands(original, nand);
	const std::vector<unsigned int>& expectedNand = nand;

	std::vector<BufferCommand> nonEraseCommands;
	for (const auto& cmd : original) {
		if (cmd.op != 'E') {
			nonEraseCommands.push_back(cmd);
		}
	}

	std::vector<BufferCommand> bestCandidate = original;

	std::vector<size_t> insertPos(nonEraseCommands.size() + 1);
	for (size_t i = 0; i <= nonEraseCommands.size(); ++i) {
		insertPos[i] = i;
	}

	std::vector<std::pair<int, int>> erasePerm = mergedErasures;
	do {
		std::vector<size_t> insertionPermutation = insertPos;
		do {
			std::vector<BufferCommand> candidate = nonEraseCommands;
			for (int k = static_cast<int>(erasePerm.size()) - 1; k >= 0; --k) {
				size_t pos = insertionPermutation[k];
				int s = erasePerm[k].first;
				int e = erasePerm[k].second;
				candidate.insert(candidate.begin() + pos, BufferCommand{ 'E', (unsigned int)s, (unsigned int)(e - s + 1) });
			}

			std::vector<unsigned int> nand = loadInitialNand("ssd_nand.txt");
			applyCommands(candidate, nand);

			if (nandEqual(nand, expectedNand)) {
				this->copyBuffer(candidate);
				return;
			}

		} while (std::next_permutation(insertPos.begin(), insertPos.begin() + erasePerm.size()));

	} while (std::next_permutation(erasePerm.begin(), erasePerm.end()));

	this->copyBuffer(original);
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

