#ifndef CHESSTYPE_CHESS
#define CHESSTYPE_CHESS

#include <Windows.h>

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
DLLFUCTION VOID* InitModule(HWND hWnd,char *LibPath);


#endif
