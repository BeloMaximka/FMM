#pragma once
#include "GenericMenu.h"

class VolumesMenu : public GenericMenu<Volume>
{
	enum COLUMNS
	{
		LETTER,
		SIZE,
		FULLNESS
	};
	bool finish = false;
public:
	VolumesMenu(FileManager* fm);
	void startLoop();

	void getInput();
	void goEnter();

	void drawEntry(int pos);
	void drawPath();

	void getEntries();
};