#define _UNICODE

#include <windows.h>
#include <iostream>

int WINAPI WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow) {

    if (!OpenClipboard(NULL)) {
        std::cerr << "failed";
        return 0;
    }

    HANDLE a = GetClipboardData(CF_TEXT);
    if (a != NULL) {
        LPTSTR lptstr;
        lptstr = (LPTSTR) GlobalLock(a);
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
    }

    CloseClipboard();
    return 0;
};