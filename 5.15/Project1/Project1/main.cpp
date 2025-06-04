#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "resource.h"
#include <string.h>
#include <tchar.h>

//ʹ���ź���ͬ�������ߺ�������
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




//�����߹���
DWORD WINAPI Thread_Product(LPVOID lpParameter)
{
	TCHAR szBuffer[256];
	TCHAR sTmpBuffer[2];
	memset(szBuffer, 0, 256);

	//��ȡ��Դ�е��ַ�
	GetWindowText(hEdit_REASOURCE, szBuffer, 256);
	DWORD Len = strlen(szBuffer);

	//ÿ��ѭ�����ӻ�������ȡ��һ���ַ�
	for (size_t i = 0; i < Len; i++)
	{
		//�жϻ������Ƿ�Ϊ�գ��ȴ��������ǿ�
        WaitForSingleObject(hSemaphoreEmpty_BUFFER1, INFINITE);

		//��ȡһ���ַ���д��
		memset(sTmpBuffer, 0, 1);
		memcpy(sTmpBuffer, &szBuffer[i], 1);

		//����д������
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
		//�ȴ���������
		WaitForSingleObject(hSemaphoreFull, INFINITE);


		//������� buffer
		EnterCriticalSection(&cs);
		GetWindowText(hEdit_PRODUCTOR_BUFFER1, szBuffer,256);
		switch (dwIndex)
		{
		case 1:
			//��ȡԴ���ڵĻ���������
			GetWindowText(hEdit_EATER1_BUFFER, sOriginal, 256);
			//��szBuffer������׷�ӵ�Դ���ڵĻ��������ݺ���
			_tcscat_s(sOriginal, 256, szBuffer);

            SetWindowText(hEdit_EATER1_BUFFER, sOriginal);
			break;
		case 2:
			GetWindowText(hEdit_EATER2_BUFFER, sOriginal, 256);
			//��szBuffer������׷�ӵ�Դ���ڵĻ��������ݺ���
			_tcscat_s(sOriginal, 256, szBuffer);

			SetWindowText(hEdit_EATER2_BUFFER, sOriginal);
			break;
        case 3:
 
			GetWindowText(hEdit_EATER3_BUFFER, sOriginal, 256);
			//��szBuffer������׷�ӵ�Դ���ڵĻ��������ݺ���
			_tcscat_s(sOriginal, 256, szBuffer);

			SetWindowText(hEdit_EATER3_BUFFER, sOriginal);
			break;
		case 4:

			GetWindowText(hEdit_EATER4_BUFFER, sOriginal, 256);
			//��szBuffer������׷�ӵ�Դ���ڵĻ��������ݺ���
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

	//�����ź�����Ĭ��Ϊ1�����໺����
	hSemaphoreEmpty_BUFFER1 = CreateSemaphore(NULL, 1, 1, NULL);
    hSemaphoreFull = CreateSemaphore(NULL, 0, 1, NULL);

	InitializeCriticalSection(&cs);
	//�����������߳�
	hThread_Pro = CreateThread(NULL, 0, Thread_Product, NULL, 0, NULL);

	//�����������߳�
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
//дһ��winmain����
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DlgProc);
}
