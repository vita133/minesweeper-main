#pragma once
#include "Resource.h"
#include "cells.h"
#include "framework.h"

class Game {
private:
	int cellSize = 24, gameCoordsX = 0, gameCoordsY = 37;
public:
	bool isGameOver = false;
	int rows = 9, cols = 9, bombs = 10;
	Cell*** board = NULL;

public:
	Game();
	~Game();
	
	void startGame(HWND hWnd);
	void clearBoard();
	void restartGame(HWND hWnd);
	void setBegginer(HWND hWnd);
	void setIntermediate(HWND hWnd);
	void setExpert(HWND hWnd);
	bool detectSaveSpace(Cell* cell, HWND hWnd);
	Cell* getActiveItem(HWND hWnd);
	void onPaint(HWND hWnd, Cell* cell);
	void onLBUp(HWND hWnd);
	void onRBUp(HWND hWnd);
};