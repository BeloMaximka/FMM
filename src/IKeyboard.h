#pragma once
#include <conio.h>
class IKeyboard
{
protected:
	enum Keys
	{
		F012 = 0,
		ARROW_UP = 72,
		ARROW_DOWN = 80,
		F5 = 63,
		F = 70,
		a = 97,
		c = 99,
		d = 100,
		e = 101,
		f = 102,
		q = 113,
		r = 114,
		s = 115,
		t = 116,
		v = 118,
		x = 120,
		z = 122,
		ARROW = 224,
	};
public:
	virtual void getInput() = 0;
	bool isInput();
};