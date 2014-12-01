#ifndef CHESSTYPE_CONSIX
#define CHESSTYPE_CONSIX

#include "CNoGo.h"

#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#ifdef CHESSTYPE
#define DLLFUCTION extern "C" EXPORT
#else
#define DLLFUCTION extern "C" IMPORT
#endif


//ģ����ȷ��У��
DLLFUCTION VOID CheckModule(char *Info, char *ChessName, int *ChessType);
//��ʼ��ģ��
DLLFUCTION VOID InitModule(HWND hWnd, char *LibPath);
//��Ӧ���̴�С�ı�
DLLFUCTION VOID OnSize(RECT rtBoard);
//��������
DLLFUCTION VOID DrawBoard(HDC hDC);
//��Ӧ���Ŀ�ʼ
DLLFUCTION VOID OnRun();
//��Ӧ������������Ϣ
DLLFUCTION INT OnLButtonDown(int x, int y);
//ȷ���з�
DLLFUCTION INT OkMove(char *denCmd);
//ȡ���з�
DLLFUCTION VOID CancelMove();
//���������¼�
DLLFUCTION INT ProcessMove(char *moveCmd, char *curCmd, char *denCmd);
//��ȡ��ǰ���巽
DLLFUCTION INT GetCurPlayer();


#endif
