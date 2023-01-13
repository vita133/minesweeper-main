#include "framework.h"
#include "toolbar.h"
#include "Resource.h"
#include "resource1.h"
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

HWND hwndToolBar = NULL;

int toolbarButtons[2] = { ID_TOOL_START, ID_TOOL_KILL };

void Toolbar::initToolbar() {
    isToolbarPressed = 0;
    InitCommonControls();
}

void Toolbar::Create(HWND hWnd, HINSTANCE hInst) {
    TBBUTTON tbb[2];

    ZeroMemory(tbb, sizeof(tbb));

    tbb[0].iBitmap = 0; //���������� ����������
    tbb[0].fsState = TBSTATE_ENABLED;
    tbb[0].fsStyle = TBSTYLE_BUTTON; //��� �������� - ������
    tbb[0].idCommand = ID_TOOL_START; //��� ID ���� � ���������� WM_COMMAND

    tbb[1].iBitmap = 1;
    tbb[1].fsState = TBSTATE_ENABLED;
    tbb[1].fsStyle = TBSTYLE_BUTTON;
    tbb[1].idCommand = ID_TOOL_KILL;

    hwndToolBar = CreateToolbarEx(hWnd,
        WS_CHILD | WS_VISIBLE | WS_BORDER |
        WS_CLIPSIBLINGS | CCS_TOP |
        TBSTYLE_TOOLTIPS,
        IDC_MY_TOOLBAR,
        2,
        hInst,
        IDB_BITMAP1,
        tbb,
        2,
        24, 24, 24, 24,
        sizeof(TBBUTTON));
}

void Toolbar::OnSize(HWND hWnd) {
    RECT rc, rw;

    if (hwndToolBar) {
        GetClientRect(hWnd, &rc); 
        GetWindowRect(hwndToolBar, &rw); 
        MoveWindow(hwndToolBar, 0, 0, rc.right - rc.left, rw.bottom - rw.top, FALSE);
    }
}

void Toolbar::OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    LPNMHDR pnmh = (LPNMHDR)lParam;

    if (pnmh->code == TTN_NEEDTEXT) {
        LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT)lParam;
        switch (lpttt->hdr.idFrom) {
        case ID_TOOL_START:
            lstrcpy(lpttt->szText, L"Start Game");
            break;
        case ID_TOOL_KILL:
            lstrcpy(lpttt->szText, L"Killl Game");
            break;

        default: lstrcpy(lpttt->szText, L"Undefined");
        }
    }
}

void Toolbar::OnTool(HWND hWnd, int id) {
    int length = sizeof(toolbarButtons) / sizeof(*toolbarButtons);
    for (int i = 0; i < length; i++) {
        int el = toolbarButtons[i];
        if (el == id) SendMessage(hwndToolBar, TB_PRESSBUTTON, id, 1);
        else SendMessage(hwndToolBar, TB_PRESSBUTTON, el, 0);
    }
}