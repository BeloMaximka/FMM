#pragma once
#include "Renderer.h"
#include "FileManager.h"
#include "IKeyboard.h"
#include <vector>
#include <array>
using namespace std;

#define COLUMNS_COUNT 3

template <class T>
class ExplorerBox : public IKeyboard
{
protected:
	short column_len[COLUMNS_COUNT]{ 32,20,12 };
	short entries_count = 10;
	int shift_pos = 0;
	int selected_pos = 0;

	COORD pos{};
	vector<T> entries;

	FileManager* fm;

	void goUp();
	void goDown();
	virtual void goEnter() = 0;

	void drawBox();
	void drawColumnNames(array<string, COLUMNS_COUNT> names);
	void drawEntries();
	void clearEntries();
	virtual void drawEntry(int pos) = 0;
	virtual void drawPath();
	void drawInfo(string line);
public:
	int getBoxHeight();
	int getBoxWidth();

	void drawAll(array<string, COLUMNS_COUNT> names);
	virtual void getInput() = 0;
};

template<class T>
inline void ExplorerBox<T>::goUp()
{
	if (selected_pos == 0)
	{
		if (shift_pos != 0)
		{
			shift_pos--;
			drawEntries();
		}
	}
	else
	{
		selected_pos--;
		drawEntry(selected_pos + 1);
		drawEntry(selected_pos);
	}
}

template<class T>
inline void ExplorerBox<T>::goDown()
{
	if (shift_pos + selected_pos + 1 >= entries.size()) return;
	if (selected_pos + 1 == entries_count)
	{
		if (shift_pos + selected_pos + 1 != entries.size())
		{
			shift_pos++;
			drawEntries();
		}
	}
	else
	{
		selected_pos++;
		drawEntry(selected_pos - 1);
		drawEntry(selected_pos);
	}
}

template<class T>
inline int ExplorerBox<T>::getBoxHeight()
{
	return 10 + entries_count;
}

template<class T>
inline int ExplorerBox<T>::getBoxWidth()
{
	return column_len[0] + column_len[1] + column_len[2] + 4;
}

template<class T>
inline void ExplorerBox<T>::drawAll(array<string, COLUMNS_COUNT> names)
{
	drawBox();
	drawColumnNames(names);
	clearEntries();
	drawEntries();
	drawPath();
}

template<class T>
inline void ExplorerBox<T>::drawBox()
{
	int width = getBoxWidth();
	COORD draw_pos = pos;

	Renderer::drawBox(draw_pos, width, 5);
	draw_pos.Y += 2;
	Renderer::drawHLine(draw_pos, width, true);
	draw_pos.Y += 2;

	draw_pos.X = pos.X;
	draw_pos.Y = pos.Y + entries_count + 7;
	Renderer::drawBox(draw_pos, width, 3);

	draw_pos.Y = pos.Y + 4;
	Renderer::drawBox(draw_pos, width, entries_count + 4);
	draw_pos.Y -= 2;
	draw_pos.X += column_len[0] + 1;
	Renderer::drawVLine(draw_pos, entries_count + 6, true);
	draw_pos.X += column_len[1] + 1;
	Renderer::drawVLine(draw_pos, entries_count + 6, true);



	draw_pos.Y += 2;
	draw_pos.X = pos.X;
	SetConsoleCursorPosition(Renderer::h, draw_pos);
	cout << (char)204;

	for (int i = 0; i < COLUMNS_COUNT - 1; i++)
	{
		draw_pos.X += column_len[i] + 1;
		SetConsoleCursorPosition(Renderer::h, draw_pos);
		cout << (char)206;
	}
	draw_pos.X += column_len[COLUMNS_COUNT - 1] + 1;
	SetConsoleCursorPosition(Renderer::h, draw_pos);
	cout << (char)185;


	draw_pos.X = pos.X;
	draw_pos.Y = pos.Y + entries_count + 7;
	SetConsoleCursorPosition(Renderer::h, draw_pos);
	cout << (char)204;
	for (int i = 0; i < COLUMNS_COUNT - 1; i++)
	{
		draw_pos.X += column_len[i] + 1;
		SetConsoleCursorPosition(Renderer::h, draw_pos);
		cout << (char)202;
	}
	draw_pos.X += column_len[COLUMNS_COUNT - 1] + 1;
	SetConsoleCursorPosition(Renderer::h, draw_pos);
	cout << (char)185;
}

template<class T>
inline void ExplorerBox<T>::drawColumnNames(array<string, COLUMNS_COUNT> names)
{
	COORD draw_pos = pos;
	draw_pos.Y += 3;
	draw_pos.X++;
	for (int i = 0; i < COLUMNS_COUNT; i++)
	{
		SetConsoleCursorPosition(Renderer::h, draw_pos);
		names[i].resize(column_len[i], ' ');
		cout << names[i];
		draw_pos.X += column_len[i] + 1;
	}
}

template<class T>
inline void ExplorerBox<T>::drawEntries()
{
	COORD draw_pos = pos;
	draw_pos.X++;
	if (entries.size() > entries_count)
	{
		if (shift_pos == 0)
		{
			draw_pos.Y += entries_count + 6;
			SetConsoleCursorPosition(Renderer::h, draw_pos);
			string line(3, (char)25);
			line.resize(column_len[0], ' ');
			cout << line;
			drawEntry(entries_count);
		}
		else
		{
			draw_pos.Y += 5;
			SetConsoleCursorPosition(Renderer::h, draw_pos);
			string line(3, (char)24);
			line.resize(column_len[0], ' ');
			cout << line;

			draw_pos.Y += entries_count + 1;
			SetConsoleCursorPosition(Renderer::h, draw_pos);
			line.replace(0, 3, 3, (char)25);
			cout << line;
			if (shift_pos == entries.size() - entries_count)
			{
				drawEntry(-1);
			}
		}
		for (int i = 0; i < entries_count; i++)
		{
			drawEntry(i);
		}

	}
	else
	{
		for (int i = 0; i < entries.size(); i++)
		{
			drawEntry(i);
		}
	}
}

template<class T>
inline void ExplorerBox<T>::clearEntries()
{
	string line;
	line.resize(column_len[0], ' ');
	line += (char)186;
	line.resize(column_len[0] + column_len[1] + 1, ' ');
	line += (char)186;
	line.resize(getBoxWidth() - 2, ' ');

	COORD draw_pos = this->pos;
	draw_pos.X++;
	draw_pos.Y += 4;
	for (int i = 0; i < entries_count + 2; i++)
	{
		draw_pos.Y++;
		SetConsoleCursorPosition(Renderer::h, draw_pos);
		cout << line;
	}
}

template<class T>
inline void ExplorerBox<T>::drawPath()
{
	COORD draw_pos = pos;
	draw_pos.X++;
	draw_pos.Y++;
	SetConsoleCursorPosition(Renderer::h, draw_pos);
	string path = fm->getCurentPath();
	int width = getBoxWidth() - 2;
	if (path.size() < width)
	{
		path.resize(width, ' ');
	}
	else if (path.size() > width)
	{
		path.erase(0, path.size() - width + 3);
		path.insert(0, 3, '.');
	}
	cout << path;
}

template<class T>
inline void ExplorerBox<T>::drawInfo(string line)
{
	COORD draw_pos = pos;
	draw_pos.X++;
	draw_pos.Y += entries_count + 8;
	SetConsoleCursorPosition(Renderer::h, draw_pos);
	line.resize(getBoxWidth() - 2, ' ');
	cout << line;
}