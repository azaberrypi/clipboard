#define _UNICODE

#include <windows.h>
#include <iostream>

int WINAPI WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow) {
    /*https://docs.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-openclipboard*/
    if (!OpenClipboard(NULL)) {
        std::cerr << "failed at OpenClipboard";
        return 0;
    }

    /*https://docs.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-getclipboarddata*/
    HANDLE a = GetClipboardData(CF_TEXT);
    if (a != NULL) {
        LPTSTR lptstr;
        lptstr = (LPTSTR) GlobalLock(a);    // nanikore?
        if (lptstr != NULL) {
            /*ここで処理*/
            std::cout << lptstr << std::endl;    // debug
            int len = lstrlen(lptstr);
            std::cout << len << std::endl;
            GlobalUnlock(a);
            WCHAR text[32];
            wsprintf(text, L"%d", len);
            MessageBox(NULL, text, L"文字列の長さ", MB_OK|MB_ICONINFORMATION);
        }
    } else {
        std::cerr << "failed at GetClipboardData";
    }

    CloseClipboard();
    return 0;
};