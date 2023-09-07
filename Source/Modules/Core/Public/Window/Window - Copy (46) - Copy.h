#pragma once
#include "windows.h"


class IshakWindow
{
public:
	IshakWindow() = default;


private:
	HWND* handler;
	int size{0 };
	int x, y;
};