#include "Renderer.h"

HANDLE Renderer::h = GetStdHandle(STD_OUTPUT_HANDLE);

void Renderer::drawBox(COORD pos, int width, int height)
{
	SetConsoleCursorPosition(h, pos);
	cout << (char)201;
	for (int i = 0; i < width - 2; i++) cout << (char)205;
	cout << (char)187;
	for (int i = 0; i < height - 2; i++)
	{
		pos.Y++;
		SetConsoleCursorPosition(h, pos);
		cout << (char)186;
	}
	pos.Y++;
	SetConsoleCursorPosition(h, pos);
	cout << (char)200;
	for (int i = 0; i < width - 2; i++) cout << (char)205;
	cout << (char)188;
	pos.X += width - 1;
	for (int i = 0; i < height - 2; i++)
	{
		pos.Y--;
		SetConsoleCursorPosition(h, pos);
		cout << (char)186;
	}
}

void Renderer::init(short height, short width)
{
	setlocale(LC_CTYPE, "C");
	SetConsoleTextAttribute(h, 7);

	SetConsoleTitle(TEXT("FMM"));
	//Вырубаем выделение
	HANDLE hInput;
	DWORD prev_mode;
	hInput = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hInput, &prev_mode);
	SetConsoleMode(hInput, prev_mode & ENABLE_EXTENDED_FLAGS);
	// Задаем размеры
	string command = "mode con cols=";
	command += std::to_string(width + 1);
	command += " lines=";
	command += std::to_string(height);
	system(command.c_str());
	// Убираем каретку
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(h, &cci);
	cci.bVisible = false;
	SetConsoleCursorInfo(h, &cci);

	system("cls");
}

void Renderer::drawVLine(COORD pos, int length, bool intersec)
{
	SetConsoleCursorPosition(h, pos);
	int i = 0;
	if (intersec)
	{
		cout << (char)203;
		i++;
		pos.Y++;
		SetConsoleCursorPosition(h, pos);
	}
	for (; i < length; i++)
	{
		cout << (char)186;
		pos.Y++;
		SetConsoleCursorPosition(h, pos);
	}
	if (intersec)
	{
		pos.Y--;
		SetConsoleCursorPosition(h, pos);
		cout << (char)202;
	}

}

void Renderer::drawHLine(COORD pos, int length, bool intersec)
{
	SetConsoleCursorPosition(h, pos);
	for (int i = 0; i < length; i++) cout << (char)205;
	if (intersec)
	{
		SetConsoleCursorPosition(h, pos);
		cout << (char)204;
		pos.X += length - 1;
		SetConsoleCursorPosition(h, pos);
		cout << (char)185;
	}
}

void Renderer::setColor(Colors color)
{
	SetConsoleTextAttribute(h, (int)color);
}
