#define _UNICODE
#define UNICODE

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
    HANDLE a = GetClipboardData(CF_UNICODETEXT);
    if (a != NULL) {
        LPTSTR lptstr_copy;
        lptstr_copy = (LPTSTR) GlobalLock(a);    // メモリブロックを確保し、最初の1Byteへのポインタを取得
        if (lptstr_copy != NULL) {
            /* process here */
            int len = lstrlen(lptstr_copy);     // the length of the string, in characters
            std::cout << len << std::endl;
            // std::cout << lstrlen(L"こんにちは abc");  // debug
            GlobalUnlock(a);
            WCHAR text[32];
            wsprintf(text, L"%d", len); // ? todo:
            MessageBox(NULL, text, L"文字列の長さ", MB_OK|MB_ICONINFORMATION);
        }
    } else {
        std::cerr << "failed at GetClipboardData";
    }

    CloseClipboard();
    return 0;
};