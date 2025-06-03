#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include "resource.h"
#include <stdio.h>



HWND hEDIT;
HANDLE hThread;

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
    //获取子窗口数值
    TCHAR szBuffer[10];
    DWORD dwTimer;
    memset(szBuffer, 0, 10);
    GetWindowText(hEDIT, szBuffer, 10);
    //转成整型
    sscanf(szBuffer, "%d", &dwTimer);
    while (dwTimer > 0)
    {

        Sleep(1000);
        sprintf(szBuffer, "%d", --dwTimer);

        SetWindowText(hEDIT, szBuffer);
    }
    return 0;
}



BOOL CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case  WM_INITDIALOG:
        hEDIT = GetDlgItem(hWnd, IDC_EDIT_CONTENT);

		SetWindowText(hEDIT, TEXT("1000"));

		return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_START:
            if (hThread == NULL) {

                hThread = CreateThread(NULL, 0, ThreadProc, 0, 0, NULL);
                return TRUE;

            }
            else {
                return TRUE;
            }
                
            
        case IDC_BUTTON_SUSPEND:
            //挂起
            SuspendThread(hThread);
            return TRUE;
        case IDC_BUTTON_RESTART:
           //  重启
            ResumeThread(hThread);
            return TRUE;
        case IDC_BUTTON_TERMINATE:
            //停止
            TerminateThread(hThread, 0);
            return TRUE;

        }
        break;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        break;
    }
    return FALSE;
}


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{ 
    DialogBoxA(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN),NULL, DialogProc);

}


