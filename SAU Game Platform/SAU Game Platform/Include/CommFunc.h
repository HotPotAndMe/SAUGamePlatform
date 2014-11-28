#ifndef CHINESE_H
#define CHINESE_H

#include <windows.h>
#include <stdio.h>
#include <ocidl.h>
#include <olectl.h>

#pragma comment(lib,"winmm.lib")

VOID __cdecl DelFile(char *dir);
BOOL __cdecl DisplayImage(HDC hDC, LPCTSTR szImagePath,RECT *rt);//չʾbmp��jpg��gif����һ֡��ͼƬ
VOID __cdecl PlayMusic(char *filename,bool circle,int vol);
VOID __cdecl StopMusic(char *filename);
VOID __cdecl ErrorBox(LPTSTR ErrorInfo);//������ʾ��
VOID __cdecl MsgBox(LPTSTR msg);//��Ϣ��
BOOL __cdecl PrintScreen(char *filename,int left,int top,int width,int height);//��ͼ
BOOL __cdecl PrintScreenEx(char *filename,HWND hWnd,bool (*DrawBoard)(HDC hDC,HDC hMemDC));
HWND __cdecl GetProcessMainWnd(DWORD dwProcessId);
VOID __stdcall GetSystemTimeEx(SYSTEMTIME *st,int region);

#endif