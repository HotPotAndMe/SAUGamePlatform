/**
 * file: GameType.h
 * date: 2014/9/16
 * version: 1.0
 * description: ���ֹ���ͷ�ļ�
 */


#ifndef GAMETYPE_H_GAMEPLATFORM
#define GAMETYPE_H_GAMEPLATFORM

#include "Game Platform.h"



//���ְ���ʶ����50500��ʼ���ϵ�����
#define CHESSTYPE 50500


//����֧��ģ��������
struct _CHESSTYPE{
	HINSTANCE chessTP;//���ְ���Դʵ�����
	char chessStr[20];//������
	int type;//��������  0���������걸��Ϣ����
};


//ģ����ȷ��У��
typedef VOID (*_CheckModule)(char *Info, char *ChessName, int *ChessType);
//��ʼ��ģ��
typedef VOID (*_InitModule)(HWND hWnd, GameSet *gameset);
//��Ӧ���̴�С�ı�
typedef VOID (*_OnSize)(RECT rtBoard);
//��������
typedef VOID (*_DrawBoard)(HDC hDC);
//��Ӧ���Ŀ�ʼ
typedef VOID (*_OnRun)();
//��Ӧ������������Ϣ
typedef INT (*_OnLButtonDown)(int x, int y);
//ȷ���з�
typedef INT (*_OkMove)(char *denCmd);
//ȡ���з�
typedef VOID (*_CancelMove)();
//���������¼�
typedef INT (*_ProcessMove)(char *moveCmd, char *curCmd, char *denCmd);
//��ȡ��ǰ���巽
typedef INT (*_GetCurPlayer)();



extern _CheckModule CT_CheckModule;
extern _InitModule CT_InitModule;
extern _OnSize CT_OnSize;
extern _DrawBoard CT_DrawBoard;
extern _OnLButtonDown CT_OnLButtonDown;
extern _OkMove CT_OkMove;
extern _CancelMove CT_CancelMove;
extern _ProcessMove CT_ProcessMove;
extern _OnRun CT_OnRun;
extern _GetCurPlayer CT_GetCurPlayer;

extern struct _CHESSTYPE chessType[25];//����֧��ģ������
extern int chesstype;//��������֧��ģ������

BOOL GetChessTypeResourse();//��ȡ����֧��ģ����Դ
VOID CreateChessTypeMenu(HMENU hMenu);//���������Ӳ˵�
VOID InitialChessType(int i,HMENU hMenu);//��ʼ�����ֽӿ�
VOID SetChessType(int i,HMENU hMenu);//��������


#endif