#include "main.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	IW64P_t IW64P = (IW64P_t)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "IsWow64Process");
	if (IW64P)
		IW64P(GetCurrentProcess(), &_WinMain.isLongMode);
	_WinMain.hInstance = hInstance;

	_MainDialog.hCursorHand = LoadCursor(0, IDC_HAND);
	_MainDialog.hBitmap = LoadBitmap(_WinMain.hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

	_AboutDialog.hBitmap = LoadBitmap(_WinMain.hInstance, MAKEINTRESOURCE(IDB_BITMAP2));

	LoadDriver();
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, MainDialog);
	UnloadDriver();
	return 0;
}

INT WINAPI MainDialog(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int ret = 0;

	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			_MainDialog.hPicture = GetDlgItem(hwndDlg, IDC_STATIC1);
			_MainDialog.hButtonStart = GetDlgItem(hwndDlg, IDC_BUTTON1);
			_MainDialog.hButtonStop = GetDlgItem(hwndDlg, IDC_BUTTON2);
			break;
		}

		case WM_PAINT:
		{
			BITMAP bm;
			PAINTSTRUCT ps;
			RECT rc;
			HDC hdc;
			HDC hdcMem;
			HBITMAP hbmOld;

			rc.right = 201;
			rc.bottom = 243;

			hdc = BeginPaint(hwndDlg, &ps);
			FillRect(ps.hdc, &ps.rcPaint, CreateSolidBrush(RGB(250, 250, 250)));
			EndPaint(hwndDlg, &ps);

			hdc = BeginPaint(_MainDialog.hPicture, &ps);
			hdcMem = CreateCompatibleDC(hdc);
			hbmOld = SelectObject(hdcMem, _MainDialog.hBitmap);
			GetObject(_MainDialog.hBitmap, sizeof(bm), &bm);
			MapDialogRect(hwndDlg, &rc);
			SetStretchBltMode(hdc, STRETCH_HALFTONE);
			StretchBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			SelectObject(hdcMem, hbmOld);
			DeleteDC(hdcMem);
			EndPaint(hwndDlg, &ps);
			break;
		}

		case WM_SETCURSOR:
		{
			if ((HWND)wParam == _MainDialog.hPicture)
			{
				SetCursor(_MainDialog.hCursorHand);
				SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, TRUE);
				ret = 1;
			}

			break;
		}

		case WM_COMMAND:
		{
			if (HIWORD(wParam) != BN_CLICKED)
				break;

			switch (LOWORD(wParam))
			{
				case IDC_STATIC1:
					if (!_AboutDialog.isOpened)
					{
						_AboutDialog.isOpened = !_AboutDialog.isOpened;
						DialogBox(_WinMain.hInstance, MAKEINTRESOURCE(IDD_DIALOG2), 0, AboutDialog);
						ret = 1;
					}

					break;

				case IDC_BUTTON1:
					if (!_MainDialog.hThread)
					{
						wchar_t ErrCode[50];

						_MainDialog.hDriver = CreateFile(DRIVER_SYMBOLIC_LINK, GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
						if (_MainDialog.hDriver == INVALID_HANDLE_VALUE)
						{
							swprintf_s(ErrCode, 50, L"%d", GetLastError());
							MessageBox(0, ErrCode, L"Error CreateFile...", MB_OK | MB_ICONERROR);
							break;
						}

						_Key[0].isChecked  = SendDlgItemMessage(hwndDlg, IDC_CHECK1,  BM_GETCHECK, 0, 0);
						_Key[1].isChecked  = SendDlgItemMessage(hwndDlg, IDC_CHECK2,  BM_GETCHECK, 0, 0);
						_Key[2].isChecked  = SendDlgItemMessage(hwndDlg, IDC_CHECK3,  BM_GETCHECK, 0, 0);
						_Key[3].isChecked  = SendDlgItemMessage(hwndDlg, IDC_CHECK4,  BM_GETCHECK, 0, 0);
						_Key[4].isChecked  = SendDlgItemMessage(hwndDlg, IDC_CHECK5,  BM_GETCHECK, 0, 0);
						_Key[5].isChecked  = SendDlgItemMessage(hwndDlg, IDC_CHECK6,  BM_GETCHECK, 0, 0);
						_Key[6].isChecked  = SendDlgItemMessage(hwndDlg, IDC_CHECK7,  BM_GETCHECK, 0, 0);
						_Key[7].isChecked  = SendDlgItemMessage(hwndDlg, IDC_CHECK8,  BM_GETCHECK, 0, 0);
						_Key[8].isChecked  = SendDlgItemMessage(hwndDlg, IDC_CHECK9,  BM_GETCHECK, 0, 0);
						_Key[9].isChecked  = SendDlgItemMessage(hwndDlg, IDC_CHECK10, BM_GETCHECK, 0, 0);
						_Key[10].isChecked = SendDlgItemMessage(hwndDlg, IDC_CHECK11, BM_GETCHECK, 0, 0);
						_Key[11].isChecked = SendDlgItemMessage(hwndDlg, IDC_CHECK12, BM_GETCHECK, 0, 0);

						BOOL success;
						UINT delay;

						delay = GetDlgItemInt(hwndDlg, IDC_EDIT1, &success, 0);
						if (success)
							_Key[0].NextDelay = delay;
						delay = GetDlgItemInt(hwndDlg, IDC_EDIT2, &success, 0);
						if (success)
							_Key[1].NextDelay = delay;
						delay = GetDlgItemInt(hwndDlg, IDC_EDIT3, &success, 0);
						if (success)
							_Key[2].NextDelay = delay;
						delay = GetDlgItemInt(hwndDlg, IDC_EDIT4, &success, 0);
						if (success)
							_Key[3].NextDelay = delay;
						delay = GetDlgItemInt(hwndDlg, IDC_EDIT5, &success, 0);
						if (success)
							_Key[4].NextDelay = delay;
						delay = GetDlgItemInt(hwndDlg, IDC_EDIT6, &success, 0);
						if (success)
							_Key[5].NextDelay = delay;
						delay = GetDlgItemInt(hwndDlg, IDC_EDIT7, &success, 0);
						if (success)
							_Key[6].NextDelay = delay;
						delay = GetDlgItemInt(hwndDlg, IDC_EDIT8, &success, 0);
						if (success)
							_Key[7].NextDelay = delay;
						delay = GetDlgItemInt(hwndDlg, IDC_EDIT9, &success, 0);
						if (success)
							_Key[8].NextDelay = delay;
						delay = GetDlgItemInt(hwndDlg, IDC_EDIT10, &success, 0);
						if (success)
							_Key[9].NextDelay = delay;
						delay = GetDlgItemInt(hwndDlg, IDC_EDIT11, &success, 0);
						if (success)
							_Key[10].NextDelay = delay;
						delay = GetDlgItemInt(hwndDlg, IDC_EDIT12, &success, 0);
						if (success)
							_Key[11].NextDelay = delay;

						for (int i = 0; i < sizeof(_Key) / sizeof(Key_t); i++)
							if (_Key[i].NextDelay < 1) _Key[i].NextDelay = 1;

						EnableWindow(_MainDialog.hButtonStart, 0);
						EnableWindow(_MainDialog.hButtonStop, 1);
						_MainDialog.hThread = CreateThread(0, 0, &WorkThread, 0, 0, 0);
						ret = 1;
					}

					break;

				case IDC_BUTTON2:
					if (_MainDialog.hThread)
					{
						TerminateThread(_MainDialog.hThread, 0);
						CloseHandle(_MainDialog.hDriver);
						EnableWindow(_MainDialog.hButtonStart, 1);
						EnableWindow(_MainDialog.hButtonStop, 0);
						_MainDialog.hThread = 0;
						ret = 1;
					}

					break;
			}

			break;
		}

		case WM_CLOSE:
		{
			EndDialog(hwndDlg, 0);
			if (_AboutDialog.isOpened)
				EndDialog(_AboutDialog.hWnd, 0);

			break;
		}
	}

	return ret;
}

INT WINAPI AboutDialog(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			_AboutDialog.hWnd = hwndDlg;
			_AboutDialog.hPicture = GetDlgItem(hwndDlg, IDC_STATIC2);
			PlaySound(MAKEINTRESOURCE(IDR_WAVE1), 0, SND_RESOURCE | SND_ASYNC | SND_LOOP);
			return 0;
		}

		case WM_PAINT:
		{	
			BITMAP bm;
			PAINTSTRUCT ps;
			RECT rc;
			HDC hdc;
			HDC hdcMem;
			HBITMAP hbmOld;

			rc.right = 310;
			rc.bottom = 210;

			hdc = BeginPaint(_AboutDialog.hPicture, &ps);
			hdcMem = CreateCompatibleDC(hdc);
			hbmOld = SelectObject(hdcMem, _AboutDialog.hBitmap);
			GetObject(_AboutDialog.hBitmap, sizeof(bm), &bm);
			MapDialogRect(hwndDlg, &rc);
			SetStretchBltMode(hdc, STRETCH_HALFTONE);
			StretchBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			SelectObject(hdcMem, hbmOld);
			DeleteDC(hdcMem);
			EndPaint(hwndDlg, &ps);
			return 0;
		}

		case WM_CLOSE:
		{
			EndDialog(hwndDlg, 0);
			PlaySound(0, 0, 0);
			_AboutDialog.isOpened = !_AboutDialog.isOpened;
			return 0;
		}
	}

	return 0;
}

DWORD WINAPI WorkThread(LPVOID lpThreadParameter)
{
	wchar_t ErrCode[50];
	DWORD bW;

	for (;;)
	{
		for (int i = 0; i < sizeof(_Key) / sizeof(Key_t); i++)
		{
			if (!_Key[i].isChecked)
				continue;

			if (!WriteFile(_MainDialog.hDriver, &_Key[i].KeyCode, sizeof(_Key[0].KeyCode), &bW, 0))
			{
				swprintf_s(ErrCode, 50, L"%d", GetLastError());
				MessageBox(0, ErrCode, L"Error CreateFile...", MB_OK | MB_ICONERROR);
				return 0;
			}

			Sleep(_Key[i].NextDelay);
		}
	}
}

int LoadDriver()
{
	wchar_t ErrCode[50];

	SC_HANDLE sch = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
	if (!sch)
	{
		swprintf_s(ErrCode, 50, L"%d", GetLastError());
		MessageBox(0, ErrCode, L"(1)Error OpenSCManager...", MB_OK | MB_ICONERROR);
		return 0;
	}

	wchar_t DriverPath[MAX_PATH];
	if (_WinMain.isLongMode)
		GetFullPathName(DRIVER_PATH64, MAX_PATH, DriverPath, NULL);
	else 
		GetFullPathName(DRIVER_PATH, MAX_PATH, DriverPath, NULL);

	SC_HANDLE schService = CreateService(sch,
		SERVICE_NAME, SERVICE_NAME, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, DriverPath, 0, 0, 0, 0, 0);
	if (!schService)
	{
		if (GetLastError() != ERROR_SERVICE_EXISTS)
		{
			swprintf_s(ErrCode, 50, L"%d", GetLastError());
			MessageBox(0, ErrCode, L"(1)Error CreateService (1)...", MB_OK | MB_ICONERROR);
			return 0;
		}

		UnloadDriver();
		schService = CreateService(sch,
			SERVICE_NAME, SERVICE_NAME, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
			SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, DriverPath, 0, 0, 0, 0, 0);
		if (!schService)
		{
			swprintf_s(ErrCode, 50, L"%d", GetLastError());
			MessageBox(0, ErrCode, L"(1)Error CreateService (2)...", MB_OK | MB_ICONERROR);
			return 0;
		}
	}

	if (!StartService(schService, 0, 0))
	{
		swprintf_s(ErrCode, 50, L"%d", GetLastError());
		MessageBox(0, ErrCode, L"(1)Error StartService...", MB_OK | MB_ICONERROR);
		return 0;
	}

	if (!CloseServiceHandle(schService))
	{
		swprintf_s(ErrCode, 50, L"%d", GetLastError());
		MessageBox(0, ErrCode, L"(1)Error CloseServiceHandle (1)...", MB_OK | MB_ICONERROR);
		return 0;
	}

	if (!CloseServiceHandle(sch))
	{
		swprintf_s(ErrCode, 50, L"%d", GetLastError());
		MessageBox(0, ErrCode, L"(1)Error CloseServiceHandle (2)...", MB_OK | MB_ICONERROR);
		return 0;
	}

	return 1;
}

int UnloadDriver()
{
	wchar_t ErrCode[50];

	SC_HANDLE sch = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
	if (!sch)
	{
		swprintf_s(ErrCode, 50, L"%d", GetLastError());
		MessageBox(0, ErrCode, L"(2)Error OpenSCManager...", MB_OK | MB_ICONERROR);
		return 0;
	}

	wchar_t DriverPath[MAX_PATH];
	if (_WinMain.isLongMode)
		GetFullPathName(DRIVER_PATH64, MAX_PATH, DriverPath, NULL);
	else
		GetFullPathName(DRIVER_PATH, MAX_PATH, DriverPath, NULL);

	SC_HANDLE schService = OpenService(sch, SERVICE_NAME, SERVICE_ALL_ACCESS);
	if (!schService)
	{
		swprintf_s(ErrCode, 50, L"%d", GetLastError());
		MessageBox(0, ErrCode, L"(2)Error CreateService...", MB_OK | MB_ICONERROR);
		return 0;
	}

	SERVICE_STATUS ServiceStatus;
	if (!ControlService(schService, SERVICE_CONTROL_STOP, &ServiceStatus))
	{
		if (GetLastError() != ERROR_SERVICE_NOT_ACTIVE)
		{
			swprintf_s(ErrCode, 50, L"%d", GetLastError());
			MessageBox(0, ErrCode, L"(2)Error ControlService...", MB_OK | MB_ICONERROR);
			return 0;
		}
	}

	if (!DeleteService(schService))
	{
		swprintf_s(ErrCode, 50, L"%d", GetLastError());
		MessageBox(0, ErrCode, L"(2)Error DeleteService...", MB_OK | MB_ICONERROR);
		return 0;
	}

	if (!CloseServiceHandle(schService))
	{
		swprintf_s(ErrCode, 50, L"%d", GetLastError());
		MessageBox(0, ErrCode, L"(2)Error CloseServiceHandle (1)...", MB_OK | MB_ICONERROR);
		return 0;
	}

	if (!CloseServiceHandle(sch))
	{
		swprintf_s(ErrCode, 50, L"%d", GetLastError());
		MessageBox(0, ErrCode, L"(2)Error CloseServiceHandle (2)...", MB_OK | MB_ICONERROR);
		return 0;
	}

	return 1;
}