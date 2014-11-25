#ifndef CHESSTYPE_CONSIX
#define CHESSTYPE_CONSIX

#include "CConSix.h"

#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#ifdef CHESSTYPE
#define DLLFUCTION extern "C" EXPORT
#else
#define DLLFUCTION extern "C" IMPORT
#endif


//ģ����ȷ��У��
DLLFUCTION VOID CheckModule(char *Info,char *ChessName,int *ChessType);
//��ʼ��ģ��
DLLFUCTION VOID InitModule(HWND hWnd,GameSet *gameset);
//��Ӧ���̴�С�ı�
DLLFUCTION VOID OnSize(RECT rt);
//��������
DLLFUCTION VOID DrawBoard(HDC hDC);
//��Ӧ������������Ϣ
DLLFUCTION BOOL OnLButtonDown(int x,int y,char *wDMsg);
//������Ϣ����
DLLFUCTION BOOL ProcessMsg(char *msg,char *wMMsg,char *wDMsg);
//��Ӧ���Ŀ�ʼ
DLLFUCTION VOID OnRun();
//��ȡ��ǰ���巽
DLLFUCTION int GetCurPlayer();


#endif
