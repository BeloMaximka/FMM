#include "IKeyboard.h"

bool IKeyboard::isInput()
{
	return _kbhit();
}