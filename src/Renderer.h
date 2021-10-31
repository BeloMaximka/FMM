#pragma once
#include "Windows.h"
#include <iostream>
#include <string>
using namespace std;
namespace Renderer
{
	enum class Colors
	{
		BASIC = 7,
		INVERTED = 240,
		HIGHLIGHTED = 2,
		HIGHLIGHTED_INVERTED = 242
	};

	extern HANDLE h;
	void init(short height, short width);
	void drawBox(COORD pos, int width, int height);
	void drawVLine(COORD pos, int length, bool intersec = false);
	void drawHLine(COORD pos, int length, bool intersec = false);
	void setColor(Colors color);
};
