#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "resource.h"
#include <string.h>
#include <tchar.h>

//使用信号量同步消费者和生产者
HWND hEdit_REASOURCE;
HWND hEdit_PRODUCTOR_BUFFER1;
HWND hEdit_PRODUCTOR_BUFFER2;
HWND hEdit_EATER1_BUFFER;
HWND hEdit_EATER2_BUFFER;
HWND hEdit_EATER3_BUFFER;
HWND hEdit_EATER4_BUFFER;
HANDLE hSemaphoreEmpty_BUFFER1;
HANDLE hSemaphoreFull;
CRITICAL_SECTION cs;
HANDLE hThread_Pro;
HANDLE hThread_Consumer1;
HANDLE hThread_Consumer2;
HANDLE hThread_Consumer3;
HANDLE hThread_Consumer4;




//生成者过程
DWORD WINAPI Thread_Product(LPVOID lpParameter)
{
	TCHAR szBuffer[256];
	TCHAR sTmpBuffer[2];
	memset(szBuffer, 0, 256);

	//获取资源中的字符
	GetWindowText(hEdit_REASOURCE, szBuffer, 256);
	DWORD Len = strlen(szBuffer);

	//每次循环，从缓冲区中取出一个字符
	for (size_t i = 0; i < Len; i++)
	{
		//判断缓冲区是否为空，等待缓冲区非空
        WaitForSingleObject(hSemaphoreEmpty_BUFFER1, INFINITE);

		//获取一个字符，写入
		memset(sTmpBuffer, 0, 1);
		memcpy(sTmpBuffer, &szBuffer[i], 1);

		//互斥写入区域
		EnterCriticalSection(&cs);
		SetWindowText(hEdit_PRODUCTOR_BUFFER1, sTmpBuffer);
		SetWindowText(hEdit_PRODUCTOR_BUFFER2, "1");
        LeaveCriticalSection(&cs);

		Sleep(1000);
		ReleaseSemaphore(hSemaphoreFull, 1, NULL);

	}



	return 0;
}
DWORD WINAPI Thread_Consumer(LPVOID lpParameter)
{
	TCHAR szBuffer[256];
	TCHAR sOriginal[256];
	DWORD dwIndex = (DWORD)lpParameter;
	while (TRUE)
	{
		//等待缓冲区满
		WaitForSingleObject(hSemaphoreFull, INFINITE);


		//互斥访问 buffer
		EnterCriticalSection(&cs);
		GetWindowText(hEdit_PRODUCTOR_BUFFER1, szBuffer,256);
		switch (dwIndex)
		{
		case 1:
			//获取源窗口的缓冲区数据
			GetWindowText(hEdit_EATER1_BUFFER, sOriginal, 256);
			//将szBuffer的数据追加到源窗口的缓冲区数据后面
			_tcscat_s(sOriginal, 256, szBuffer);

            SetWindowText(hEdit_EATER1_BUFFER, sOriginal);
			break;
		case 2:
			GetWindowText(hEdit_EATER2_BUFFER, sOriginal, 256);
			//将szBuffer的数据追加到源窗口的缓冲区数据后面
			_tcscat_s(sOriginal, 256, szBuffer);

			SetWindowText(hEdit_EATER2_BUFFER, sOriginal);
			break;
        case 3:
 
			GetWindowText(hEdit_EATER3_BUFFER, sOriginal, 256);
			//将szBuffer的数据追加到源窗口的缓冲区数据后面
			_tcscat_s(sOriginal, 256, szBuffer);

			SetWindowText(hEdit_EATER3_BUFFER, sOriginal);
			break;
		case 4:

			GetWindowText(hEdit_EATER4_BUFFER, sOriginal, 256);
			//将szBuffer的数据追加到源窗口的缓冲区数据后面
			_tcscat_s(sOriginal, 256, szBuffer);

			SetWindowText(hEdit_EATER4_BUFFER, sOriginal);
			break;
		}
		SetWindowText(hEdit_PRODUCTOR_BUFFER2, "0");
		LeaveCriticalSection(&cs);
		Sleep(1000);
		ReleaseSemaphore(hSemaphoreEmpty_BUFFER1, 1, NULL);
	}


	
	return 0;
}

DWORD WINAPI MainThread(LPVOID lpParameter)
{

	//创建信号量，默认为1个空余缓冲区
	hSemaphoreEmpty_BUFFER1 = CreateSemaphore(NULL, 1, 1, NULL);
    hSemaphoreFull = CreateSemaphore(NULL, 0, 1, NULL);

	InitializeCriticalSection(&cs);
	//创建生产者线程
	hThread_Pro = CreateThread(NULL, 0, Thread_Product, NULL, 0, NULL);

	//创建消费者线程
	hThread_Consumer1 = CreateThread(NULL, 0, Thread_Consumer, (void*)1, 0, NULL);
	hThread_Consumer2 = CreateThread(NULL, 0, Thread_Consumer, (void*)2, 0, NULL);
	hThread_Consumer3 = CreateThread(NULL, 0, Thread_Consumer, (void*)3, 0, NULL);
	hThread_Consumer4 = CreateThread(NULL, 0, Thread_Consumer, (void*)4, 0, NULL);
	WaitForSingleObject(hThread_Pro, INFINITE);
	WaitForSingleObject(hThread_Consumer1, INFINITE);
	WaitForSingleObject(hThread_Consumer2, INFINITE);
	WaitForSingleObject(hThread_Consumer3, INFINITE);
	WaitForSingleObject(hThread_Consumer4, INFINITE);
    CloseHandle(hThread_Pro);
    CloseHandle(hThread_Consumer1);
	CloseHandle(hThread_Consumer2);
	CloseHandle(hThread_Consumer3);
	CloseHandle(hThread_Consumer4);


	DeleteCriticalSection(&cs);
	return 0;
}



BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hDlg, 0);
		break;
	}
	case WM_INITDIALOG:
	{
		hEdit_REASOURCE = GetDlgItem(hDlg, IDC_EDIT_REASOURCE);
		hEdit_PRODUCTOR_BUFFER1 = GetDlgItem(hDlg, IDC_EDIT_BUFFER1);
		hEdit_PRODUCTOR_BUFFER2 = GetDlgItem(hDlg, IDC_EDIT_BUFFER2);
		hEdit_EATER1_BUFFER = GetDlgItem(hDlg, IDC_EDIT_EATER1_BUFFER);
		hEdit_EATER2_BUFFER = GetDlgItem(hDlg, IDC_EDIT_EATER2_BUFFER);
		hEdit_EATER3_BUFFER = GetDlgItem(hDlg, IDC_EDIT_EATER3_BUFFER);
		hEdit_EATER4_BUFFER = GetDlgItem(hDlg, IDC_EDIT_EATER4_BUFFER);
		break;
	}
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_START:
		{
			CreateThread(NULL, 0, MainThread, NULL, 0, NULL);

			return TRUE;
		}
		}
		break;
	}

	return 0;
}
//写一个winmain函数
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DlgProc);
}
