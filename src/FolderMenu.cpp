#include "FolderMenu.h"
#include "VolumesMenu.h"

FolderMenu::FolderMenu(FileManager* fm)
{
	this->fm = fm;
	getEntries();
}

void FolderMenu::drawError(FileManager::Code code)
{
	switch (code)
	{
	case FileManager::Code::DOESNT_EXIST:
		drawInfo("This entry doesn't exist.");
		break;
	case FileManager::Code::PATH_UNREACHABLE:
		drawInfo("Path is unreachable.");
		break;
	case FileManager::Code::ALREADY_EXIST:
		drawInfo("The entry already exist.");
		break;
	case FileManager::Code::ACCESS_DENIED:
		drawInfo("Access denied.");
		break;
	case FileManager::Code::UNKNOWN:
		drawInfo("Unknown error.");
		break;
	}
}

bool FolderMenu::isRestricted(char c)
{
	char restricted[] = "\\/:*?\"<>|";
	for (auto temp : restricted)
	{
		if (c == temp) return true;
	}
	return false;
}

string FolderMenu::getStringInput(string begin)
{
	COORD draw_pos = pos;
	draw_pos.X++;
	draw_pos.Y += entries_count + 8;
	drawInfo("");
	SetConsoleCursorPosition(Renderer::h, draw_pos);
	cout << begin << ": ";
	int max_size = getBoxWidth() - begin.size() - 4;
	string name;
	while (true)
	{
		int code = _getch();
		if (code == 224)
		{
			_getch();
			continue;
		}
		if (!isRestricted(code) && code >= 32 && code <= 126 && name.size() < max_size)
		{
			name += (char)code;
			cout << (char)code;
		}
		else if (code == 8 && name.size() > 0)
		{
			name.pop_back();
			cout << "\b \b";
		}
		else if (code == 13)
		{
			return name;
		}
	}
}

bool FolderMenu::confirm(string str)
{
	COORD draw_pos = pos;
	draw_pos.X++;
	draw_pos.Y += entries_count + 8;
	SetConsoleCursorPosition(Renderer::h, draw_pos);
	cout << str << "[Y/N]";
	while (true)
	{
		int code = _getch();
		if (code == 224)
		{
			_getch();
			continue;
		}
		if (code == 'y' || code == 'Y')
			return true;
		if (code == 'n' || code == 'N')
			return false;
	}
}

void FolderMenu::getInput()
{
	int code = _getch();
	if (code == ARROW || code == F012) code = _getch();
	switch (code)
	{
	case ARROW_UP:
		goUp();
		break;
	case ARROW_DOWN:
		goDown();
		break;
	case q:
		goBack();
		break;
	case e:
		goEnter();
		break;
	case a:
		switchHighlighted();
		if (isBufferEmpty())
			operation = Operations::NONE;
		drawEntry(selected_pos);
		break;
	case z:
		operation = Operations::NONE;
		clearBuffer();

		drawEntries();
		drawInfo("Cleared.");
		break;
	case F5:
		drawInfo("Refreshed.");
		updateEntries();
		break;
	case s:
		showSize();
		break;
	case c:
		if (!isBufferEmpty())
		{
			operation = Operations::COPY;
			drawInfo("Selected to copy.");
		}
		break;
	case x:
		if (!isBufferEmpty())
		{
			operation = Operations::CUT;
			drawInfo("Selected to move.");
		}

		break;
	case v:
		if (operation != Operations::NONE)
			paste();
		break;
	case r:
		rename();
		break;
	case d:
		deleteCurrent();
		break;
	case f:
		createFolder();
		break;
	case F:
		createFile();
		break;
	case t:
		find();
		break;
	}
}

void FolderMenu::goBack()
{
	if (fm->back() == FileManager::Code::ROOT)
	{
		VolumesMenu vb(fm);
		vb.startLoop();
	}
	shift_pos = 0;
	selected_pos = 0;
	FileManager::Code code = fm->getDirectoryContents(entries);
	if (code != FileManager::Code::SUCCESS)
	{
		goBack();
		drawError(code);
		return;
	}
	drawColumnNames({ "Name", "Date", "Size" });
	clearEntries();
	drawEntries();
	drawPath();
}

void FolderMenu::goEnter()
{
	if (fm->enter(entries[shift_pos + selected_pos].name) == FileManager::Code::DOESNT_EXIST)
	{
		drawInfo("This entry doesn't exist.");
	}
	updateEntries();
}

string FolderMenu::getFullPathToSelected()
{
	return fm->getCurentPath() + entries[shift_pos + selected_pos].name;
}

string FolderMenu::getFullPath(int pos)
{
	return fm->getCurentPath() + entries[shift_pos + pos].name;
}

void FolderMenu::showSize()
{
	string path = getFullPathToSelected();
	unsigned long long size;
	if (fm->size(path, size) == FileManager::Code::DOESNT_EXIST)
	{
		drawInfo("This entry doesn't exist.");
		updateEntries();
	}
	else drawInfo("Size: " + fm->formatSize(size));
}

void FolderMenu::paste()
{
	int count = buffer.size();
	int errors = 0;
	for (auto& entry : buffer)
	{
		FileManager::Code code;
		if (operation == Operations::CUT)
			code = fm->move(entry);
		else
			code = fm->copy(entry);
		if (code == FileManager::Code::SUCCESS)
		{
			if (operation == Operations::COPY)
				drawInfo("Copied.");
			else
				drawInfo("Moved.");
		}
		else
			drawError(code);
	}
	buffer.clear();
	updateEntries();
}

void FolderMenu::rename()
{
	string name = getStringInput("Rename");
	FileManager::Code code = fm->rename(getFullPathToSelected(), name);
	if (code == FileManager::Code::SUCCESS)
		drawInfo("Renamed.");
	else
		drawError(code);
	updateEntries();
}

void FolderMenu::deleteCurrent()
{
	if (confirm("Confirm deletion:"))
	{
		FileManager::Code code = fm->deleteFileOrFolder(getFullPathToSelected());
		if (code == FileManager::Code::SUCCESS)
			drawInfo("Deleted.");
		else
			drawError(code);
		updateEntries();
	}
	else
		drawInfo("Canceled.");
}

void FolderMenu::createFolder()
{
	string name = getStringInput("New folder name");
	FileManager::Code code = fm->createFolder(name);
	if (code == FileManager::Code::SUCCESS)
		drawInfo("Folder created.");
	else
		drawError(code);
	updateEntries();
}

void FolderMenu::createFile()
{
	string name = getStringInput("New file name");
	FileManager::Code code = fm->createFile(name);
	if (code == FileManager::Code::SUCCESS)
		drawInfo("File created.");
	else
		drawError(code);
	updateEntries();
}

void FolderMenu::find()
{
	string name = getStringInput("To find");
	FileManager::Code code = fm->find(name);
	if (code == FileManager::Code::SUCCESS)
		drawInfo("Found in this directory.");
	else if (code == FileManager::Code::FAILED)
		drawInfo("Nothing found.");
	else
		drawError(code);
	updateEntries();
}

void FolderMenu::getEntries()
{
	FileManager::Code code = fm->getDirectoryContents(entries);
	if (code != FileManager::Code::SUCCESS)
	{
		goBack();
		drawError(code);
	}
}

void FolderMenu::updateEntries()
{
	shift_pos = 0;
	selected_pos = 0;
	FileManager::Code code = fm->getDirectoryContents(entries);
	if (code != FileManager::Code::SUCCESS)
	{
		goBack();
		drawError(code);
	}
	clearEntries();
	drawEntries();
	drawPath();
}

void FolderMenu::addToBuffer(string full_path)
{
	buffer.insert(full_path);
}

void FolderMenu::removeFromBuffer(string full_path)
{
	buffer.erase(full_path);
}

void FolderMenu::clearBuffer()
{
	buffer.clear();
}

bool FolderMenu::isInBuffer(string full_path)
{
	return buffer.find(full_path) != buffer.end();
}

bool FolderMenu::isBufferEmpty()
{
	return buffer.size() == 0;
}

void FolderMenu::switchHighlighted()
{
	string path = getFullPathToSelected();

	if (isInBuffer(path)) removeFromBuffer(path);
	else addToBuffer(path);
}

void FolderMenu::drawEntry(int pos)
{
	auto& entry = entries[shift_pos + pos];

	if (pos == selected_pos)
	{
		if (isInBuffer(getFullPath(pos)))
			Renderer::setColor(Renderer::Colors::HIGHLIGHTED_INVERTED);
		else
			Renderer::setColor(Renderer::Colors::INVERTED);
	}
	else if (isInBuffer(getFullPath(pos)))
		Renderer::setColor(Renderer::Colors::HIGHLIGHTED);
	else
		Renderer::setColor(Renderer::Colors::BASIC);

	COORD draw_pos = this->pos;
	draw_pos.X++;
	draw_pos.Y += pos + 5;
	if (entries.size() > entries_count && shift_pos != 0) draw_pos.Y++;
	if (entries.size() > entries_count && shift_pos == entries.size() - entries_count) draw_pos.Y++;
	SetConsoleCursorPosition(Renderer::h, draw_pos);

	string line;
	if (entry.name.size() > column_len[NAME])
	{
		string temp = entry.name;
		temp.resize(column_len[NAME] - 3);
		temp += "...";
		line += temp;
	}
	else
	{
		line += entry.name;
		line.resize(column_len[NAME], ' ');
	}
	line += (char)186;
	line += entry.date;
	line += ' ';
	line += (char)186;
	if (!entry.isFolder)
	{
		line += fm->formatSize(entry.size);
	}
	line.resize(column_len[NAME] + column_len[DATE] + column_len[SIZE] + 2, ' ');
	cout << line;
	Renderer::setColor(Renderer::Colors::BASIC);
}