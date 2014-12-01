/**
* file: Manual.cpp
* date: 2014/11/29
* version: 1.0
* description:
*/


#include "Manual.h"


//��ͼģ��
//=================================================================================================================

/**
 * PSDrawBoard - ��������
 * @hDC:	Ŀ���豸����DC
 * description:	PSDrawBoard��CMainWnd�������̲��ֻ�ͼ�����ķ�װ�������ƹ�˽�г�Ա����ʵ�ֺ���ָ��Ĵ��ݡ�PSDrawBoard������CMainWnd�����Ԫ������
 */
void PSDrawBoard(HDC hDC)
{
	MainWnd->FillBkAndBoard(hDC);
	CT_DrawBoard(hDC);
}

/**
 * PrintScreenOnChess - �����̽��н�ͼ
 * @filename:	Ŀ���ļ���
 * return:		TRUE:����ɹ���FALSE:����ʧ��
 */
BOOL PrintScreenOnChess(char *filename)
{
	return PrintScreenEx(filename, MainWnd->hWnd, MainWnd->GetBoardPos(), PSDrawBoard);
}

/**
 * PrintScrOnTemp - ��ݽ�ͼ
 */
VOID PrintScrOnTemp()
{
	char filename[MAX_PATH] = { 0 };
	char time[100] = { 0 };

	SYSTEMTIME st;
	GetSystemTimeEx(&st, 8);//��ȡϵͳʱ��
	sprintf(time, "\\%d-%d-%d-%d-%d-%d-%d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	strcpy(filename, ".");//gameSet.CurDir);
	strcat(filename, "\\PrintScr");
	strcat(filename, time);
	strcat(filename, ".bmp");
	PrintScreenOnChess(filename);
	return;
}

/**
 * SavePrintScreen - ���������ͼ
 * description:	ʵ��SavePrintScreen��PrintScrOnTemp�ĺ��ļ�������ȫһ�µģ�ֻ��������ʵ�ֹ��ܵĶ�λ�ϲ�ͬ��PrintScrOnTemp����֧���û��ֶ������Ŀ�ݽ�ͼ��
 *				SavePrintScreen����֧����������û��趨��ɵ��Զ���ͼ��
 *				�Զ���ͼ��ԭ��������֧��ģ�鴦��һ����Ч���ŷ�����ʱ������SendMessage������MainWnd���ڷ���GM_SHOWSTEP��Ϣ����֪�����ŷ�������Ч�Լ�������ŷ���wParam��������
 *								ƽ̨�ڴ���GM_SHOWSTEP��Ϣʱ�����SavePrintScreen������ɱ��������ͼ������
 */
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
		if (gameSet.PrintScrDir[0] == 0)
		{
			strcpy(gameSet.PrintScrDir, ".");//gameSet.CurDir);
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

/**
 * SaveChessManual - ��������
 * @step:			��ǰ��Ч�����ŷ�
 * description:		���ܶ�λ��ʵ��ԭ��ͬSavePrintScreen������ͬ��
 */
VOID SaveChessManual(char *step)
{
	char filename[MAX_PATH];
	char time[100];
	HANDLE hFile;
	DWORD dwWrite;
	SYSTEMTIME st;

	if (gameSet.cmDir[0] == 0)
	{
		strcpy(gameSet.cmDir, ".");//gameSet.CurDir);
		strcat(gameSet.cmDir, "\\chess manual\\");
		strcat(gameSet.cmDir, chessType[chesstype].chessStr);
		strcat(gameSet.cmDir, "\\sgf");
	}
	strcpy(filename, gameSet.cmDir);
	strcat(filename, "\\");

	if (gameSet.SingleStep)//��������
	{
		::GetSystemTimeEx(&st, 8);//��ȡϵͳʱ��  
		sprintf(time, "%d-%d-%d-%d-%d-%d-%d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		strcat(filename, time);
		strcat(filename, ".sgf");

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
		if (!WriteFile(hFile, step, strlen(step), &dwWrite, NULL))
		{
			ErrorBox("WriteFile failed");
			CloseHandle(hFile);
			return;
		}
		CloseHandle(hFile);
	}

	if (gameSet.AllStep)//����ȫ������
	{
		if (gameSet.cmFileName[0] == 0)
		{
			strcpy(gameSet.cmFileName, "Untitled");
		}
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
		if (hFile == INVALID_HANDLE_VALUE)
		{
			ErrorBox("CreateFile failed");
			return;
		}
		SetFilePointer(hFile, 0, NULL, FILE_END);//�����ļ�����ָ��Ϊ�ļ���β
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