//Implementing multi-threaded packet grabbing using mutexes

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "resource.h"
#include <stdio.h>

HWND hEDIT_Total;
HWND hEDIT_Grab1;
HWND hEDIT_Grab2;
HWND hEDIT_Grab3;
HANDLE hMutex;


DWORD WINAPI Grab_Process(HWND GrabHandle)
{
	TCHAR szBuffer[10];
	TCHAR szBuffer1[10];
	DWORD dwInt;
	DWORD dwInt1;
	memset(szBuffer, 0, 10);
	memset(szBuffer1, 0, 10);
	while (true)
	{
		WaitForSingleObject(hMutex, INFINITE);
		//Get remaining packets
		GetWindowText(hEDIT_Total, szBuffer, 10);
		sscanf(szBuffer, "%d", &dwInt);

		//Get current packet count
		GetWindowText(GrabHandle, szBuffer1, 10);
		sscanf(szBuffer1, "%d", &dwInt1);

		if (dwInt != 0)
		{
			dwInt = dwInt - 50;
			dwInt1 = dwInt1 + 50;
			sprintf(szBuffer, "%d", dwInt);
			sprintf(szBuffer1, "%d", dwInt1);

			//rewrite the edit boxes
			SetWindowText(hEDIT_Total, szBuffer);
			SetWindowText(GrabHandle, szBuffer1);
			ReleaseMutex(hMutex);
			Sleep(1000);

		}
		else {
			break;
		}
	}
	return 2;

}


DWORD WINAPI Grab1(LPVOID lpParameter)
{
	Grab_Process(hEDIT_Grab1);
	return 0;
}
DWORD WINAPI Grab2(LPVOID lpParameter)
{
	Grab_Process(hEDIT_Grab2);
	return 0;
}
DWORD WINAPI Grab3(LPVOID lpParameter)
{
	Grab_Process(hEDIT_Grab3);
	return 0;
}





DWORD WINAPI GrabLoader(LPVOID lpParameter)
{
	hMutex = CreateMutex(NULL, FALSE, "xyz");

	// grab packets here
	HANDLE Grab1_thread = CreateThread(NULL, 0, Grab1, NULL, 0, NULL);
	HANDLE Grab2_thread = CreateThread(NULL, 0, Grab2, NULL, 0, NULL);
	HANDLE Grab3_thread = CreateThread(NULL, 0, Grab3, NULL, 0, NULL);
	if (WaitForSingleObject(Grab1_thread, INFINITE)) {
		CloseHandle(Grab1_thread);
	};
	if (WaitForSingleObject(Grab2_thread, INFINITE)) {
		CloseHandle(Grab2_thread);
	};
	if (WaitForSingleObject(Grab3_thread, INFINITE)) {
		CloseHandle(Grab3_thread);
	};
	return 0;
}



BOOL CALLBACK DialogProc(
	HWND hwndDlg,  // handle to dialog box			
	UINT uMsg,     // message			
	WPARAM wParam, // first message parameter			
	LPARAM lParam  // second message parameter			
)
{

	switch (uMsg)
	{
	case  WM_INITDIALOG:
	{

		//TCHAR szBuffer[5];

		hEDIT_Total = GetDlgItem(hwndDlg, IDC_EDIT_TOTAL);
		hEDIT_Grab1 = GetDlgItem(hwndDlg, IDC_EDIT_GRAB1);
		hEDIT_Grab2 = GetDlgItem(hwndDlg, IDC_EDIT_GRAB2);
		hEDIT_Grab3 = GetDlgItem(hwndDlg, IDC_EDIT_GRAB3);

		//initialize edit boxes
		SetWindowText(hEDIT_Total, TEXT("1000"));
		SetWindowText(hEDIT_Grab1, TEXT("0"));
        SetWindowText(hEDIT_Grab2, TEXT("0"));
		SetWindowText(hEDIT_Grab3, TEXT("0"));
		return TRUE;
	}
	case  WM_CLOSE:

		EndDialog(hwndDlg, 0);

		return TRUE;

	case  WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case   IDC_BUTTON_GRAB:
			HANDLE hThread = CreateThread(NULL, 0, GrabLoader, 0, 0, NULL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}



int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBoxA(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProc);
}