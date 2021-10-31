#include "VolumesBox.h"

VolumesBox::VolumesBox(FileManager* fm)
{
	this->fm = fm;
	getEntries();
}

void VolumesBox::startLoop()
{
	drawAll({ "Letter", "Capacity", "Free space" });
	while (!finish)
	{
		if (isInput())
		{
			getInput();
		}
	}
}

void VolumesBox::getInput()
{
	int code = _getch();
	if (code == ARROW) code = _getch();
	switch (code)
	{
	case ARROW_UP:
		goUp();
		break;
	case ARROW_DOWN:
		goDown();
		break;
	case e:
		goEnter();
		break;
	}
}

void VolumesBox::goEnter()
{
	fm->enter(entries[shift_pos + selected_pos].letter + string(":"));
	finish = true;
}

void VolumesBox::drawEntry(int pos)
{
	if (pos == selected_pos)
		Renderer::setColor(Renderer::Colors::INVERTED);
	else
		Renderer::setColor(Renderer::Colors::BASIC);

	auto& entry = entries[shift_pos + pos];

	COORD draw_pos = this->pos;
	draw_pos.X++;
	draw_pos.Y += pos + 5;
	if (entries.size() > entries_count && shift_pos != 0) draw_pos.Y++;
	if (entries.size() > entries_count && shift_pos == entries.size() - entries_count) draw_pos.Y++;
	SetConsoleCursorPosition(Renderer::h, draw_pos);

	string line;

	line += entry.letter;
	line += ":\\";
	line.resize(column_len[LETTER], ' ');

	line += (char)186;
	line += fm->formatSize(entry.size);
	line.resize(column_len[LETTER] + column_len[SIZE] + 1, ' ');

	line += (char)186;
	line += to_string(entry.fullness) + '%';
	line.resize(column_len[LETTER] + column_len[SIZE] + column_len[FULLNESS] + 2, ' ');

	cout << line;
	Renderer::setColor(Renderer::Colors::BASIC);
}

void VolumesBox::drawPath()
{
	COORD draw_pos = pos;
	draw_pos.X++;
	draw_pos.Y++;
	SetConsoleCursorPosition(Renderer::h, draw_pos);
	cout << "My computer";
}

void VolumesBox::getEntries()
{
	entries = fm->getDisks();
}