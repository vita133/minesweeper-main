#include <windows.h>
#include <windowsx.h>
#include "framework.h"
#include "cells.h"
#include "resource1.h"


bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hdc, int destX, int destY, int width, int height, int srcX, int srcY);


void Cell::setCoords(int newX, int newY, int mapI, int mapJ) {
	x = newX;
	y = newY;
	i = mapI;
	j = mapJ;
}

Field::Field() {
	mines = 0;
	isClicked = false;
	isMine = false;
	isFlagged = false;
}

void Field::SetView(HWND hWnd, int blockSize) {
	HDC hdc = GetDC(hWnd);
	if (isFlagged) {
		LoadAndBlitBitmap(__T("bitmap1.bmp"), hdc, x, y, 24, 24, field_flag, 0);
	}
	else if (isClicked) {
		LoadAndBlitBitmap(__T("bitmap1.bmp"), hdc, x, y, 24, 24, (5 + mines) * 24, 0);
	}
	else {
		LoadAndBlitBitmap(__T("bitmap1.bmp"), hdc, x, y, 24, 24, field_default, 0);
	}
}

Mine::Mine() {
	mines = 0;
	isMine = true;
	isClicked = false;
	isFlagged = false;
}

void Mine::SetView(HWND hWnd, int blockSize) {
	HDC hdc = GetDC(hWnd);
	if (isFlagged) {
		LoadAndBlitBitmap(__T("bitmap1.bmp"), hdc, x, y, 24, 24, field_flag, 0);
	}
	else if (isClicked) {
		LoadAndBlitBitmap(__T("bitmap1.bmp"), hdc, x, y, 24, 24, field_bomb, 0);
	}
	else {
		LoadAndBlitBitmap(__T("bitmap1.bmp"), hdc, x, y, 24, 24, field_default, 0);
	}
}

bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hdc, int destX, int destY, int width, int height, int srcX, int srcY) {
	HBITMAP hBitmap;
	hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hBitmap == NULL) {
		::MessageBox(NULL, __T("LoadImage Failed"), __T("Error"), MB_OK);
		return 0;
	}

	HDC hLocalDC;
	hLocalDC = ::CreateCompatibleDC(hdc);
	if (hLocalDC == NULL) {
		::MessageBox(NULL, __T("CreateCompatibleDC Failed"), __T("Error"), MB_OK);
		return 0;
	}

	BITMAP qBitmap;
	int iReturn = GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP), reinterpret_cast<LPVOID>(&qBitmap));
	if (iReturn == NULL) {
		::MessageBox(NULL, __T("GetObject Failed"), __T("Error"), MB_OK);
		return 0;
	}

	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hLocalDC, hBitmap);
	if (hOldBmp == NULL) {
		::MessageBox(NULL, __T("SelectObject Failed"), __T("Error"), MB_OK);
		return 0;
	}

	BOOL qRetBlit = ::BitBlt(hdc, destX, destY, width, height, hLocalDC, srcX, srcY, SRCCOPY);
	if (qRetBlit == NULL) {
		::MessageBox(NULL, __T("BitBlt Failed"), __T("Error"), MB_OK);
		return 0;
	}

	::SelectObject(hLocalDC, hOldBmp);
	::DeleteDC(hLocalDC);
	::DeleteObject(hBitmap);
	return 1;
}

