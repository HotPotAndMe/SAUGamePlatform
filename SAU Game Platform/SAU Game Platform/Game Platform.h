/**
* file: Game Platform.h
* date: 2014/7/15
* version: 2.0
* description: 
*/


#ifndef MAIN_H_GAMEPLATFORM
#define MAIN_H_GAMEPLATFORM


//��̬�⾲̬����
//============================================================================
#pragma comment(lib,"WSOCK32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")

#ifdef _DEBUG

#pragma comment(lib,"..\\CommFunc\\Debug\\CommFunc.lib")
#pragma comment(lib,"..\\FileDlg\\Debug\\FileDlg.lib")

#else

#pragma comment(lib,"..\\CommFunc\\Release\\CommFunc.lib")
#pragma comment(lib,"..\\FileDlg\\Release\\FileDlg.lib")

#endif


#include <Windows.h>
#include <stdio.h>
#include <algorithm>
#include <map>
#include "resource.h"
#include "re_function.h"
#include "Set.h"
#include "..\CommFunc\CommFunc.h"
#include "..\FileDlg\FileDlg.h"

using namespace std;


//��
#define BLACK 0
#define WHITE 1

#define MsgBox(lpText,lpCaption,dwMilliseconds) \
	{ if(MessageBoxTimeout==NULL||dwMilliseconds==0) MessageBox(NULL,lpText,lpCaption,MB_OK); \
	  else MessageBoxTimeout(NULL,lpText,lpCaption,MB_OK,0,dwMilliseconds); }
//������ʾ��
#define ErrorBox(ErrorInfo) \
	{ CHAR error1[50],error2[20]; \
	  strcpy(error1,ErrorInfo); \
	  sprintf(error2,"\n\nerror: %d",GetLastError()); \
	  strcat(error1,error2); \
	  MessageBox(NULL,error1,"error",MB_OK); }
//�����ı����ı�����
#define SetText(hWnd,str) \
	{ SendMessage(hWnd,WM_SETTEXT,(WPARAM)0,(LPARAM)str); /*�������*/  \
	  SendMessage(hWnd,EM_SETSEL,(WPARAM)-2,(LPARAM)-1); /*���ý��㵽ĩβ*/ \
	  SendMessage(hWnd,WM_VSCROLL,SB_BOTTOM,0); }
//��ȡ�ı����ı�����
#define GetText(hWnd,str,size) \
	{ SendMessage(hWnd,WM_GETTEXT,size,(LPARAM)str); }


//�Զ�����Ϣ
#define GM_NAME			0X1011		//����������
#define GM_SHOWSTEP		0X1012		//��ʾ�ŷ���Ϣ
#define GM_WINLOSE		0X1013		//�ֳ�ʤ��
#define GM_CREATECHAT   0X1014		//��������Э������
#define GM_DESTROYCHAT  0X1015		//��������Э������


typedef int (__stdcall *MSGBOXAAPI)(IN HWND hWnd,IN LPCSTR lpText,IN LPCSTR lpCaption,IN UINT uType,IN WORD wLanguageID,IN DWORD dwMilliseconds);
extern MSGBOXAAPI MessageBoxTimeout;

typedef int (*FUNCTION)();//����ָ��ṹ


//ȫ�ֱ���
extern HINSTANCE hInst;//����ʵ�����



#endif