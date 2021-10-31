#include "FileManager.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <chrono>

FileManager::FileManager()
{
	current = fs::current_path().string() + '\\';
}

string FileManager::formatSize(unsigned long long size)
{
	char letters[] = { 'K','M','G','T', 'P' };
	long double new_size = size;
	int index = -1;
	for (size_t i = 0; i < 5; i++)
	{
		if (new_size > 1024)
		{
			new_size /= 1024;
			index++;
		}
		else break;
	}
	new_size = round(new_size * 100) / 100;

	string result = std::to_string((int)new_size);
	if (new_size - (int)new_size > 0)
	{
		result += '.' + std::to_string((int)((new_size - (int)new_size) * 100));
	}
	result += ' ';
	if (index != -1) result += letters[index];
	result += 'B';
	return result;
}

FileManager::Code FileManager::fsErrCodeToFMCode(int code)const
{
	switch (code)
	{
	case 2:
		return Code::DOESNT_EXIST;
		break;
	case 5:
		return Code::ACCESS_DENIED;
		break;
	case 80:
		return Code::ALREADY_EXIST;
		break;
	default:
		return Code::UNKNOWN;
		break;
	}
}

FileManager::Code FileManager::createFolder(string name)
{
	string path = current.string() + name;
	try
	{
		fs::create_directory(path);
		return Code::SUCCESS;
	}
	catch (const fs::filesystem_error& exception)
	{
		return fsErrCodeToFMCode(exception.code().value());
	}
}

FileManager::Code FileManager::deleteFileOrFolder(string path)
{
	try
	{
		fs::remove_all(path);
		return Code::SUCCESS;
	}
	catch (const fs::filesystem_error& exception)
	{
		return fsErrCodeToFMCode(exception.code().value());
	}
}

FileManager::Code FileManager::createFile(string name_with_extension)
{
	string path = current.string() + name_with_extension;
	if (fs::exists(path))
	{
		return Code::ALREADY_EXIST;
	}
	std::ofstream(path).close();
	return Code::SUCCESS;
}

FileManager::Code FileManager::size(string path, unsigned long long& size)
{
	//string path = current.string() + path;
	if (fs::exists(path))
	{
		if (fs::is_directory(path))
		{
			size = 0;
			for (auto& entry : fs::recursive_directory_iterator(path))
			{
				size += entry.file_size();
			}
		}
		else
		{
			size = fs::file_size(path);
		}
		return Code::SUCCESS;
	}
	size = 0;
	return Code::DOESNT_EXIST;
}

FileManager::Code FileManager::back()
{
	if (current == current.root_path()) return Code::ROOT;
	current = current.parent_path();

	if (current.parent_path() != current.root_path())
	{
		current = current.parent_path().string() + '\\';
		if (!fs::exists(current)) back();
	}
	else
		current = current.parent_path().string();

	return Code::SUCCESS;
}

FileManager::Code FileManager::enter(string path)
{
	if (path.size() == 2 && path[1] == ':')
	{
		current = path + '\\';
		return Code::SUCCESS;
	}
	else
	{
		fs::path full = current.string() + path;
		if (fs::exists(full))
		{
			if (fs::is_directory(full))
			{
				current = current.string() + path + '\\';
				return Code::SUCCESS;
			}
			else
			{
				// execute
				return Code::SUCCESS;
			}
		}
		else return Code::DOESNT_EXIST;
	}
}

FileManager::Code FileManager::rename(string path, string new_name)
{
	try
	{
		fs::rename(path, current.string() + new_name);
		return Code::SUCCESS;
	}
	catch (const fs::filesystem_error& exception)
	{
		return fsErrCodeToFMCode(exception.code().value());
	}
}

FileManager::Code FileManager::copy(string src)
{
	string name = src;
	name.erase(0, name.find_last_of('\\') + 1);
	try
	{
		fs::copy(src, current.string() + name, fs::copy_options::recursive);
		return Code::SUCCESS;
	}
	catch (const fs::filesystem_error& exception)
	{
		return fsErrCodeToFMCode(exception.code().value());
	}
}

FileManager::Code FileManager::move(string src)
{
	string name = src;
	name.erase(0, name.find_last_of('\\') + 1);
	try
	{
		fs::rename(src, current.string() + name);
		return Code::SUCCESS;
	}
	catch (const fs::filesystem_error& exception)
	{
		return fsErrCodeToFMCode(exception.code().value());
	}
}

FileManager::Code FileManager::find(string name)
{
	try
	{
		for (auto& entry : fs::recursive_directory_iterator(current))
		{
			if (entry.path().filename().string() == name)
			{
				current = entry.path().parent_path();
				return Code::SUCCESS;
			}
		}
		return Code::FAILED;
	}
	catch (const fs::filesystem_error& exception)
	{
		return fsErrCodeToFMCode(exception.code().value());
	}
}

FileManager::Code FileManager::getDirectoryContents(vector<Entry>& entries)const
{
	try
	{
		entries.clear();
		for (const auto& entry : fs::directory_iterator(current))
		{
			Entry temp;
			temp.name = entry.path().filename().string();

			using namespace std::chrono;
			std::time_t time = duration_cast<seconds>(entry.last_write_time().time_since_epoch()).count();
			tm now{};
			localtime_s(&now, &time);
			char buffer[20];
			sprintf_s(buffer, "%02i.%02i.%i %02i:%02i:%02i", now.tm_mday, now.tm_mon + 1, now.tm_year + 1531, now.tm_hour, now.tm_min, now.tm_sec);
			temp.date += buffer;

			if (entry.is_directory())
			{
				temp.isFolder = true;
				temp.size = 0;
			}
			else
			{
				temp.isFolder = false;
				temp.size = entry.file_size();
			}
			entries.push_back(temp);
		}
		return Code::SUCCESS;
	}
	catch (const fs::filesystem_error& exception)
	{
		return fsErrCodeToFMCode(exception.code().value());
	}
}

vector<Volume> FileManager::getDisks() const
{
	vector<Volume> result;
	unsigned long disks = GetLogicalDrives();
	unsigned long bitmask = 1;
	for (int i = 0; i < 32; i++)
	{
		if (disks & bitmask)
		{
			Volume temp;
			temp.letter = 65 + i;

			string volume_path;
			volume_path += temp.letter;
			volume_path += ":\\";

			auto disk_info = fs::space(volume_path);
			temp.size = disk_info.capacity;
			temp.fullness = ceil((double)disk_info.free / disk_info.capacity * 100);

			result.push_back(temp);
		}
		bitmask = bitmask << 1;
	}
	return result;
}

string FileManager::getCurentPath() const
{
	return current.string();
}