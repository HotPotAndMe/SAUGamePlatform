#ifndef CHINESE_H
#define CHINESE_H

#include <windows.h>
#include <stdio.h>
#include <ocidl.h>
#include <olectl.h>

#pragma comment(lib,"winmm.lib")

BOOL __cdecl CreateFolder(char *Dir);//������Ŀ¼
VOID __cdecl DelFile(char *dir,bool all);//ɾ��ָ��Ŀ¼�µ��ļ���dll������Ŀ¼���ļ���ɾ��
BOOL __cdecl DisplayImage(HDC hDC, LPCTSTR szImagePath,RECT *rt);//չʾbmp��jpg��gif����һ֡��ͼƬ
VOID __cdecl PlayMusic(char *filename,bool circle,int vol);
VOID __cdecl StopMusic(char *filename);
VOID __cdecl ErrorBox(LPTSTR ErrorInfo);//������ʾ��
VOID __cdecl MsgBox(LPTSTR msg);//��Ϣ��
BOOL __cdecl PrintScreen(char *filename,int left,int top,int width,int height);//��ͼ
BOOL __cdecl PrintScreenEx(char *filename, HWND hWnd, RECT rtCut, void(*Draw)(HDC hDC));
HWND __cdecl GetProcessMainWnd(DWORD dwProcessId);
VOID __stdcall GetSystemTimeEx(SYSTEMTIME *st,int region);

#endif