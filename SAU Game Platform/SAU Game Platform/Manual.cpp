/**
* file: Manual.cpp
* date: 2014/11/29
* version: 1.0
* description:
*/


#include "Manual.h"


//��ͼģ��
//=================================================================================================================


void PSDrawBoard(HDC hDC)//��CBoardWnd�ĳ�Ա����DrawBoard���з�װ����ʵ�ֺ���ָ��Ĵ���
{
	MainWnd->FillBkAndBoard(hDC);
	CT_DrawBoard(hDC);
}

//�����̽��н�ͼ
BOOL PrintScreenOnChess(char *filename)
{
	return PrintScreenEx(filename, MainWnd->hWnd, MainWnd->GetBoardPos(), PSDrawBoard);
}

//��ݼ���ͼ
VOID PrintScrOnTemp()
{
	char filename[MAX_PATH] = { 0 };
	char time[100] = { 0 };

	SYSTEMTIME st;
	GetSystemTimeEx(&st, 8);//��ȡϵͳʱ��
	sprintf(time, "\\%d-%d-%d-%d-%d-%d-%d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	strcpy(filename, gameSet.CurDir);
	strcat(filename, "\\PrintScr");
	strcat(filename, time);
	strcat(filename, ".bmp");
	PrintScreenOnChess(filename);
	return;
}

//���������ͼ
VOID SavePrintScreen()
{
	SYSTEMTIME st;
	char filename[MAX_PATH] = { 0 };
	char time[100] = { 0 };

	if (gameSet.PrintScr)
	{
		GetSystemTimeEx(&st, 8);//��ȡϵͳʱ��
		sprintf(time, "\\%d-%d-%d-%d-%d-%d-%d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		char PrintScrFileName[MAX_PATH] = { 0 };
		if (gameSet.PrintScrDir == 0)
		{
			strcpy(gameSet.PrintScrDir, gameSet.CurDir);
			strcat(gameSet.PrintScrDir, "\\chess manual\\");
			strcat(gameSet.PrintScrDir, chessType[chesstype].chessStr);
			strcat(gameSet.PrintScrDir, "\\bmp");
		}
		strcpy(PrintScrFileName, gameSet.PrintScrDir);
		strcat(PrintScrFileName, time);
		strcat(PrintScrFileName, ".bmp");
		PrintScreenOnChess(PrintScrFileName);
	}
}

//��������
VOID SaveChessManual(char *step)
{
	char filename[MAX_PATH];
	char time[100];

	memset(filename, 0, sizeof(filename));
	memset(time, 0, sizeof(time));

	SYSTEMTIME st;
	::GetSystemTimeEx(&st, 8);//��ȡϵͳʱ��  
	sprintf(time, "\\%d-%d-%d-%d-%d-%d-%d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	if (gameSet.cmDir[0] == 0)
	{
		strcpy(gameSet.cmDir, gameSet.CurDir);
		strcat(gameSet.cmDir, "\\chess manual\\");
		strcat(gameSet.cmDir, chessType[chesstype].chessStr);
		strcat(gameSet.cmDir, "\\sgf");
	}
	strcpy(filename, gameSet.cmDir);
	strcat(filename, time);
	strcat(filename, ".sgf");

	HANDLE hFile;
	if (gameSet.SingleStep)
	{
		hFile = CreateFile(
			filename,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			ErrorBox("CreateFile failed");
			return;
		}
		DWORD dwWrite;
		if (!WriteFile(hFile, step, strlen(step), &dwWrite, NULL))
		{
			ErrorBox("WriteFile failed");
			CloseHandle(hFile);
			return;
		}
		CloseHandle(hFile);
	}

	if (gameSet.AllStep)
	{
		if (gameSet.cmFileName[0] == 0)
		{
			strcpy(gameSet.cmFileName, "Untitled");
		}
		memset(filename, 0, sizeof(filename));
		strcpy(filename, gameSet.cmDir);
		strcat(filename, "\\");
		strcat(filename, gameSet.cmFileName);
		strcat(filename, ".sgf");

		hFile = CreateFile(
			filename,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);

		SetFilePointer(hFile, 0, NULL, FILE_END);

		DWORD dwWrite;
		if (!WriteFile(hFile, step, strlen(step), &dwWrite, NULL) || !WriteFile(hFile, "\r\n", 2, &dwWrite, NULL))
		{
			ErrorBox("WriteFile failed");
			CloseHandle(hFile);
			return;
		}
		CloseHandle(hFile);
	}
	return;
}