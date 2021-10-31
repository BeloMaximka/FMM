#pragma once
#include "ExplorerBox.h"

class VolumesBox : public ExplorerBox<Volume>
{
	enum COLUMNS
	{
		LETTER,
		SIZE,
		FULLNESS
	};
	bool finish = false;
public:
	VolumesBox(FileManager* fm);
	void startLoop();

	void getInput();
	void goEnter();

	void drawEntry(int pos);
	void drawPath();

	void getEntries();
};