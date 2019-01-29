#include "input.h"

int Input::translateX(int& x)
{
	return x - (SCRN_WD - x);
}

int Input::translateY(int& y)
{
	return -y + (SCRN_HT - y);
}

