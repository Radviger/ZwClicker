#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <stdio.h>

#include "resource.h"

#pragma comment(lib, "winmm.lib")

#define SERVICE_NAME L"detectme_14882280_easy_fingerprint"
#define DRIVER_PATH L"ZwClicker.sys"
#define DRIVER_PATH64 L"ZwClicker64.sys"
#define DRIVER_SYMBOLIC_LINK L"\\\\.\\デバイスドライバ名はみゆきです"

typedef int (__stdcall *IW64P_t)(HANDLE, BOOL *);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
INT WINAPI MainDialog(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT WINAPI AboutDialog(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI WorkThread(LPVOID lpThreadParameter);

int LoadDriver();
int UnloadDriver();

typedef struct WinMain_s
{
	HINSTANCE hInstance;
	BOOL      isLongMode;
} WinMain_t;

typedef struct MainDialog_s
{
	HANDLE  hThread;
	HANDLE  hDriver;
	HBITMAP hBitmap;
	HWND    hPicture;
	HWND    hButtonStart;
	HWND    hButtonStop;
	HCURSOR hCursorHand;
} MainDialog_t;

typedef struct AboutDialog_s
{
	HBITMAP hBitmap;
	HWND    hPicture;
	HWND    hWnd;
	BOOL    isOpened;
} AboutDialog_t;

typedef struct Key_s
{
	UINT KeyCode;
	UINT NextDelay;
	BOOL isChecked;
} Key_t;

WinMain_t _WinMain = {0, 0};
MainDialog_t _MainDialog = {0, 0, 0, 0, 0};
AboutDialog_t _AboutDialog = {0, 0, 0, 0};
Key_t _Key[] = {{.KeyCode = 0x3B, .NextDelay = 1, .isChecked = 0},  //F1
                {.KeyCode = 0x3C, .NextDelay = 1, .isChecked = 0},  //F2
                {.KeyCode = 0x3D, .NextDelay = 1, .isChecked = 0},  //F3
                {.KeyCode = 0x3E, .NextDelay = 1, .isChecked = 0},  //F4
                {.KeyCode = 0x3F, .NextDelay = 1, .isChecked = 0},  //F5
                {.KeyCode = 0x40, .NextDelay = 1, .isChecked = 0},  //F6
                {.KeyCode = 0x41, .NextDelay = 1, .isChecked = 0},  //F7
                {.KeyCode = 0x42, .NextDelay = 1, .isChecked = 0},  //F8
                {.KeyCode = 0x43, .NextDelay = 1, .isChecked = 0},  //F9
                {.KeyCode = 0x44, .NextDelay = 1, .isChecked = 0},  //F10
                {.KeyCode = 0x57, .NextDelay = 1, .isChecked = 0},  //F11
                {.KeyCode = 0x58, .NextDelay = 1, .isChecked = 0}}; //F12