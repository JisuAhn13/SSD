#include "commandBuffer.h"
#include "SSD_func.h"

void CommandBuffer::removeTxt(std::string& token)
{
	size_t pos = token.find(".txt");
	if (pos != std::string::npos) {
		token = token.substr(0, pos);
	}
}
// 디렉토리 존재 여부 확인
bool CommandBuffer::directoryExists(const std::string& path) {
	DWORD ftyp = GetFileAttributesA(path.c_str());
	return (ftyp != INVALID_FILE_ATTRIBUTES && (ftyp & FILE_ATTRIBUTE_DIRECTORY));
}

bool CommandBuffer::fileExists(const std::string& path) {
	DWORD attr = GetFileAttributesA(path.c_str());
	return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
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

bool bufferFileExists(const std::string& fileName) {
	std::regex pattern(R"([1-5]_.*\.txt)");
	return std::regex_match(fileName, pattern);
}
Command CommandBuffer::getBufferIndex(int i) {
	return CommandBuffer::buffer[i];
}
Command CommandBuffer::getCommandFromFile(std::string fileName) {
	std::stringstream ss(fileName);
	std::string token;
	Command ret = { 0, };

	// '_'을 구분자로 문자열을 분리하여 벡터에 저장
	while (std::getline(ss, token, '_')) {

		if (token == "empty.txt") {
			removeTxt(token);
			return ret;
		}

		if (token == "W" || token == "E") {
			ret.op = token[0];
		}
		else if (ret.firstData ==0 && token.length() == 2) {
			ret.firstData = std::stoul(token);
		}
		else if( token.length() >= 2) {
			removeTxt(token);
			ret.secondData = std::stoul(token, nullptr, 16);  // 16진수 처리
		}
	}
	return ret;
}

// 생성자 
CommandBuffer::CommandBuffer() {
	std::string baseDir = "buffer";

	// buffer 폴더 생성
	if (!directoryExists(baseDir)) {
		if (!CreateDirectoryA(baseDir.c_str(), NULL)) {
			std::cerr << "Failed to create base directory: " << baseDir << std::endl;
		}
		else {
			std::cout << "Created base directory: " << baseDir << std::endl;
		}
	}

	//현재경로에 파일 읽기
	std::vector<std::string> bufferFileLists = getFileNamesInDirectory();

	bool fileChecker = false;
	for (const auto& fileName : bufferFileLists) {
		if (bufferFileExists(fileName)) {
			Command cmd = getCommandFromFile(fileName);
			buffer.push_back(cmd);
			fileChecker = true;
		}
	}
	if (fileChecker) return;

	// 1_empty.txt ~ 5_empty.txt 파일 생성
	for (int i = 1; i <= 5; ++i) {
		std::string filePath = baseDir + "\\" + std::to_string(i) + "_empty.txt";

		if (!fileExists(filePath)) {
			std::ofstream outFile(filePath);
			if (outFile) {
				std::cout << "Created file: " << filePath << std::endl;
				outFile.close();
			}
			else {
				std::cerr << "Failed to create file: " << filePath << std::endl;
			}
		}
		else {
			std::cout << "File already exists: " << filePath << std::endl;
		}
	}
}
