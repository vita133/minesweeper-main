#pragma once

class Toolbar {
private:
	int isToolbarPressed;
public:
	void initToolbar();
	void Create(HWND, HINSTANCE);
	void OnSize(HWND);
	void OnNotify(HWND, WPARAM, LPARAM);
	void OnTool(HWND, int);
};