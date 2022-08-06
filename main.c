#define UNICODE

#include <windows.h>
#include "resource.h"

// for handing over other functions
HINSTANCE hInst;
HWND hEditBoxWnd;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void CountClipBoard(HWND hWnd) {
    /*https://docs.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-openclipboard*/
    if (!OpenClipboard(hWnd)) {
        MessageBox(hWnd, TEXT("failed at OpenClipboard"), TEXT("Error"), MB_OK | MB_ICONERROR);
        return;
    }

    /*https://docs.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-getclipboarddata*/
    HANDLE a = GetClipboardData(CF_UNICODETEXT);
    if (a != NULL) {
        LPTSTR lptstr_copy;
        lptstr_copy = (LPTSTR) GlobalLock(a);    // allocate memory block and get the pointer of the first 1Byte
        if (lptstr_copy != NULL) {
            int len = lstrlen(lptstr_copy);     // the length of the string, in characters
            GlobalUnlock(a);
            WCHAR text[32];
            wsprintf(text, TEXT("%d"), len);
            //MessageBox(hWnd, text, TEXT("Length"), MB_OK | MB_ICONINFORMATION);   // debug
            SetWindowText(hEditBoxWnd, text);
        }
    } else {
        //MessageBox(hWnd, TEXT("failed at GetClipboardData"), TEXT("Error"), MB_OK | MB_ICONERROR);
        SetWindowText(hEditBoxWnd, TEXT("UNCOUNTABLE"));
    }

    CloseClipboard();
}

int WINAPI WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow) {
    TCHAR szAppName[] = TEXT("ClipBoardCounter");
    WNDCLASSEX wc;    // window class which model of window
    HWND hWnd;
    MSG msg;
    hInst = hInstance;


    // set attributes of the window class
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));  // at taskbar
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szAppName;
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));  // at window's upper left
    wc.cbSize = sizeof(wc);

    // register the window class
    if (!RegisterClassEx(&wc)) return 0;

    // create the window
    hWnd = CreateWindow(szAppName, TEXT("Clipboard_counter"),
                        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT,
                        CW_USEDEFAULT, 300, 55,
                        NULL, // handle of parent window
                        NULL, hInstance, NULL);

    if (!hWnd) return 0;

    // apply TOPMOST
    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    ShowWindow(hWnd, nCmdShow);

    UpdateWindow(hWnd);

    // message loop
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {

        case WM_CREATE: {
            RegisterHotKey(hWnd, (int) MAKEWORD(VK_CONTROL, VK_RETURN), MOD_CONTROL,
                           VK_RETURN);

            CreateWindow(TEXT("button"), TEXT("Count"),
                         WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 0, 0, 60, 25, hWnd,
                         (HMENU) IDM_ARROW, hInst, NULL);

            hEditBoxWnd =
                    CreateWindow(TEXT("EDIT"), NULL, WS_VISIBLE | WS_CHILD | ES_LEFT, 65, 4,
                                 100, 25, hWnd, NULL, hInst, NULL);

        }
            break;

        case WM_HOTKEY:
            if (MAKEWORD(VK_CONTROL, VK_RETURN) == wParam) {
                CountClipBoard(hWnd);
            }
            break;

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDM_ARROW: // button is pushed
                    CountClipBoard(hWnd);
                    break;

                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);    // Do it yourself, Windows!
    }
}