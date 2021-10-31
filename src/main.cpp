#include "FileManager.h"
#include "FolderBox.h"

int main()
{
	FileManager fm;
	FolderBox fb(&fm);
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