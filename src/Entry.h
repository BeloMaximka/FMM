#pragma once
#include <string>
struct Entry
{
	std::string name;
	std::string date;
	unsigned long long size;
	bool isFolder;
};