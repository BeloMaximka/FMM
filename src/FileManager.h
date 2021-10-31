#pragma once
#include <string>
#include <vector>
#include <filesystem>

#include "Entry.h"
#include "Volume.h"

using std::vector;
using std::string;
using std::ostream;
namespace fs = std::filesystem;
class FileManager
{
	enum class Code;
	fs::path current;
	FileManager::Code fsErrCodeToFMCode(int code)const;
public:
	enum class Code
	{
		SUCCESS,
		ROOT,
		DOESNT_EXIST,
		ALREADY_EXIST,
		ACCESS_DENIED,
		UNKNOWN,
		FAILED
	};
	FileManager();
	string formatSize(unsigned long long size);

	Code createFile(string name_with_extension);
	Code createFolder(string name);
	Code deleteFileOrFolder(string path);
	Code rename(string path, string new_name);
	Code copy(string src);
	Code move(string src);
	Code find(string name);

	Code size(string path, unsigned long long& size);

	Code back();
	FileManager::Code enter(string path);

	Code getDirectoryContents(vector<Entry>& entries)const;
	vector<Volume> getDisks()const;
	string getCurentPath()const;
};