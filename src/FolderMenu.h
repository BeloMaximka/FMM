#pragma once
#include "GenericMenu.h"
#include <set>

class FolderMenu : public GenericMenu<Entry>
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

	void drawError(FileManager::Code code);

	bool isRestricted(char c);
	string getStringInput(string begin);
	bool confirm(string str);

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
public:
	FolderMenu(FileManager* fm);
	void getInput();
};