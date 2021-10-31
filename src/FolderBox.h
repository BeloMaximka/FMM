#pragma once
#include "ExplorerBox.h"
#include <set>

class FolderBox : public ExplorerBox<Entry>
{
	enum COLUMNS
	{
		NAME,
		DATE,
		SIZE
	};
	enum class Operations
	{
		NONE,
		COPY,
		CUT
	};
	set<string> buffer;
	Operations operation = Operations::NONE;
public:
	FolderBox(FileManager* fm);
	void drawError(FileManager::Code code);

	bool isRestricted(char c);
	string getStringInput(string begin);
	bool confirm(string str);
	void getInput();
	void goBack();
	void goEnter();

	string getFullPathToSelected();
	string getFullPath(int pos);
	void showSize();
	void paste();
	void rename();
	void deleteCurrent();
	void createFolder();
	void createFile();
	void find();

	void drawEntry(int pos);
	void getEntries();
	void updateEntries();

	void addToBuffer(string full_path);
	void removeFromBuffer(string full_path);
	void clearBuffer();
	bool isInBuffer(string full_path);
	bool isBufferEmpty();
	void switchHighlighted();
};