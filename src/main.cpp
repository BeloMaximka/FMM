#include "FileManager.h"
#include "FolderMenu.h"

int main()
{
	FileManager fm;
	FolderMenu fb(&fm);
	Renderer::init(fb.getBoxHeight(), fb.getBoxWidth());
	fb.drawAll({ "Name", "Date", "Size" });
	while (true)
	{
		if (fb.isInput())
		{
			fb.getInput();
		}
	}
}