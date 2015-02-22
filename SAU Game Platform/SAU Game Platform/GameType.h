/**
 * file: GameType.h
 * date: 2014/9/16
 * version: 1.0
 * description: ���ֹ���ͷ�ļ�
 */


#ifndef GAMETYPE_H_GAMEPLATFORM
#define GAMETYPE_H_GAMEPLATFORM

#include "Game Platform.h"
#include "..\\Chess Type\\include\\CChess.h"



//���ְ���ʶ����50500��ʼ���ϵ�����
#define CHESSTYPE 50500


//����֧��ģ��������
struct _CHESSTYPE{
	HINSTANCE chessTP;//���ְ���Դʵ�����
	int type;//��������λ���  0bit���������˶��ı�ǣ�1bit�������˻����ģ�2bit������Pass��ǣ�3bit�����������ս��4bit������error�Ի���
	char chessStr[20];//������
	char LibPath[MAX_PATH];
};


//ģ����ȷ��У��
typedef VOID (*_CheckModule)(char *Info, char *ChessName, int *ChessType);
//��ʼ��ģ��
typedef VOID* (*_InitModule)(HWND hWnd, char *LibPath);


extern _CheckModule CT_CheckModule;
extern _InitModule CT_InitModule;

extern CChess *GameType;

extern struct _CHESSTYPE chessType[25];//����֧��ģ������
extern int chesstype;//��������֧��ģ������
extern int chessNum;

BOOL GetChessTypeResourse();//��ȡ����֧��ģ����Դ
VOID CreateChessTypeMenu(HMENU hMenu);//���������Ӳ˵�
VOID InitialChessType(HMENU hMenu);//��ʼ�����ֽӿ�
VOID SetChessType(int i,HMENU hMenu);//��������


#endif