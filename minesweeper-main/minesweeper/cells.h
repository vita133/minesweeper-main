#pragma once
#include "framework.h"

class Cell {
public: 
	long x, y, i, j;
	bool isMine, isClicked, isFlagged;
	int mines;
	void setCoords(int x, int y, int mapI, int mapJ);
	virtual void SetView(HWND hWnd, int cellSize) = 0;
};

class Field : public Cell {
public:
	Field();
	void SetView(HWND hWnd, int cellSize);
};

class Mine : public Cell {
public:
	Mine();
	void SetView(HWND hWnd, int cellSize);
};