#include "framework.h"
#include <math.h>
#include <windowsx.h>
#include <cstdlib>
#include <ctime>

#include "game.h"

Game::Game() {}
Game::~Game() {
	clearBoard();
}

void Game::clearBoard() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			delete board[i][j];
		}
		delete[] board[i];
	}
	delete[] board;
	board = NULL;
}

void Game::restartGame(HWND hWnd) {
	clearBoard();
	startGame(hWnd);
}
 
void Game::setBegginer(HWND hWnd) {
	clearBoard();
	rows = 9; cols = 9; bombs = 10;
	startGame(hWnd);
};

void Game::setIntermediate(HWND hWnd) {
	clearBoard();
	rows = 16; cols = 16; bombs = 40;
	startGame(hWnd);
};

void Game::setExpert(HWND hWnd) {
	clearBoard();
	rows = 16; cols = 30; bombs = 99;
	startGame(hWnd);
};

void Game::startGame(HWND hWnd) {
	isGameOver = false;

	int** boardTemplate = new int* [rows];
	for (int i = 0; i < rows; i++) {
		boardTemplate[i] = new int[cols] {0};
	}
	
	board = new Cell** [rows];
	for (int i = 0; i < rows; i++) {
		board[i] = new Cell* [cols];
	}

	// випадкова генерация бомб
	srand(time(NULL));
	for (size_t bomb = 0; bomb <= bombs; bomb++) {
		int i = rand() % rows;
		int j = rand() % cols;
		boardTemplate[i][j] = -1;
		board[i][j] = new Mine();
		board[i][j]->setCoords(gameCoordsX + j * cellSize, gameCoordsY + i * cellSize, i, j);
	}

	// задання пустих полей
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (boardTemplate[i][j] == 0) {
				board[i][j] = new Field();
				board[i][j]->setCoords(gameCoordsX + j * cellSize, gameCoordsY + i * cellSize, i, j);
			}
		}
	}

	// генерация полей-цифр
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (boardTemplate[i][j] == 0) {
				int counter = 0;
				 if (i != 0 && j != 0 && board[i - 1][j - 1]->isMine) { counter += 1; }
				 if (i != 0 && board[i - 1][j]->isMine) { counter += 1; }
				 if (i != 0 && j != cols - 1 && board[i - 1][j + 1]->isMine) { counter += 1; }

				 if (j != 0 && board[i][j - 1]->isMine) { counter += 1; }
				 if (j != cols - 1 && board[i][j + 1]->isMine) { counter += 1; }

				 if (i != rows - 1 && j != 0 && board[i + 1][j - 1]->isMine) { counter += 1; }
				 if (i != rows - 1 && board[i + 1][j]->isMine) { counter += 1; }
				 if (i != rows - 1 && j != cols - 1 && board[i + 1][j + 1]->isMine) { counter += 1; }

				boardTemplate[i][j] = counter;
				board[i][j]->mines = counter;
			}
		}
	}

	InvalidateRect(hWnd, NULL, TRUE);

}

Cell* Game::getActiveItem(HWND hWnd) {
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);
	bool clickedOnBoard = pt.x >= gameCoordsX && pt.x <= gameCoordsX + cols * cellSize &&
						  pt.y >= gameCoordsY && pt.y <= gameCoordsY + rows * cellSize;
	if (clickedOnBoard) {
		int j = floor((double)(pt.x - (double)gameCoordsX) / (double)cellSize);
		int i = floor((double)(pt.y - (double)gameCoordsY) / (double)cellSize);
		
		return board[i][j];
	}
	return nullptr;
}

bool Game::detectSaveSpace(Cell* cell, HWND hWnd) {
	//якщо з цифрою
	if (cell->mines > 0) {
		cell->isClicked = true;
		onPaint(hWnd, cell);
		return 1;
	}
	//якщо пуста
	else if (cell->mines == 0 && cell->isClicked == false) {
		const int i = cell->i;
		const int j = cell->j;
		cell->isClicked = true;
		onPaint(hWnd, cell);
		 if (i != 0 && j != 0) { detectSaveSpace(board[i - 1][j - 1], hWnd); }
		 if (i != 0) { detectSaveSpace(board[i - 1][j], hWnd); }
		 if (i != 0 && j != cols - 1) { detectSaveSpace(board[i - 1][j + 1], hWnd); }

		 if (j != 0) { detectSaveSpace(board[i][j - 1], hWnd); }
		 if (j != cols - 1) { detectSaveSpace(board[i][j + 1], hWnd); }

		 if (i != rows - 1 && j != 0) { detectSaveSpace(board[i + 1][j - 1], hWnd); }
		 if (i != rows - 1) { detectSaveSpace(board[i + 1][j], hWnd); }
		 if (i != rows - 1 && j != cols - 1) { detectSaveSpace(board[i + 1][j + 1], hWnd); }
	}
};

void Game::onLBUp(HWND hWnd) {
	Cell* clickedItem = getActiveItem(hWnd);
	// якщо ще не відкрили й без прапорця
	if (clickedItem != nullptr && clickedItem->isClicked == false && clickedItem->isFlagged == false)
	{

		if (clickedItem->isMine == false) {
			detectSaveSpace(clickedItem, hWnd);
		}

		else {
			isGameOver = true;
			clickedItem->isClicked = true;
			onPaint(hWnd, nullptr);
		}
	}
}

void Game::onRBUp(HWND hWnd) {
	Cell* clickedItem = getActiveItem(hWnd);

	if (clickedItem != nullptr && clickedItem->isClicked == false) 
	{
		// ставимо прапорець
		if (clickedItem->isFlagged == false) {
			clickedItem->isFlagged = true;
		}
		// прибираєм прапорець
		else {
			clickedItem->isFlagged = false;
		}
		onPaint(hWnd, clickedItem);
	}
}

void Game::onPaint(HWND hWnd, Cell* cell) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	if (cell == nullptr) {
		//гру закінчено
		if (isGameOver == true) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					board[i][j]->isClicked = true;
					if (board[i][j]->isMine == true) {
						board[i][j]->SetView(hWnd, cellSize);
					}
				}
			}
		}
		//відмальовуєм на початку
		else if (board != NULL) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					board[i][j]->SetView(hWnd, cellSize);
				}
			}
		}
	}
	else {
		cell->SetView(hWnd, cellSize);
	}

	EndPaint(hWnd, &ps);
}