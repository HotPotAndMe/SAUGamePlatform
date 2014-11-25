/**
* file: Game Platform.cpp
* date: 2014/7/15
* version: 2.0
* description: SAU Game Platform ��Ŀ�ļ���������ں���
*/


#include "Game Platform.h"
#include "Menu.h"
#include "CMainWnd.h"
#include "GameType.h"
#include "NetWork.h"


HINSTANCE hInst=NULL;//����ʵ�����


MSGBOXAAPI MessageBoxTimeout=NULL;

VOID GetAndDispatchMessage();//��Ϣѭ������
BOOL PrepareForApp(HINSTANCE hInstance);//����׼��



/**
 * WinMain - Win32Ӧ�ó������ڵ�
 * @hInstance:	Ӧ�ó���ʵ�����
 * @lpCmdLine:	������ָ��
 * @nShowCmd:	��ʾ����
 */
int WINAPI WinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine,_In_ int nShowCmd)
{
	PrepareForApp(hInstance);
	InitialChessType(0,hMenu);

	MainWnd=new CMainWnd;
	if(MainWnd->RegisterWnd(hInstance)==FALSE||MainWnd->CreateWnd(NULL,hMenu)==FALSE)
	{
		ErrorBox("Create MainWnd failed!");
		return -1;
	}
//	if(!NetWnd.RegisterWnd(hInstance)||!ChatWnd.RegisterWnd(hInstance))
//	{
//		ErrorBox("Register NetWnd or ChatWnd failed");
//		return 0;
//	}

	GetAndDispatchMessage();

	return 0;
}

/**
 * GetAndDispatchMessage - ��Ϣѭ��
 */
VOID GetAndDispatchMessage()
{
	HACCEL hAccTable;
	MSG msg;
	hAccTable=LoadAccelerators(hInst,MAKEINTRESOURCE(IDR_ACCELERATOR));//�����ݼ�
#ifdef _DEBUG
	MsgBox("������Ϣѭ���ɹ���","Msg",1500);
#endif

	while(GetMessage(&msg,NULL,0,0))//����Ϣ������ȡ����Ϣ
	{
		if(!TranslateAccelerator(msg.hwnd,hAccTable,&msg))//�ѿ�ݼ���Ϣת�����ַ���Ϣ
		{
			TranslateMessage(&msg);//���û�п�ݼ���Ϣ��ת��������Ϣ
			DispatchMessage(&msg);//�ɷ���Ϣ
		}
	}
	DestroyAcceleratorTable(hAccTable);//ɾ�����ټ���
}

/*
 * PrepareForApp - ����׼��
 */
BOOL PrepareForApp(HINSTANCE hInstance)
{
	hInst=hInstance;//��¼��Ӧ�ó����ʵ�����

	if(!GetCurrentDirectory(sizeof(gameSet.CurDir),gameSet.CurDir))//��ȡ����ĵ�ǰ��Ŀ¼������Ӧ�ó���ĵ�ǰĿ¼��ô�䣬CurDirʼ�ղ���
	{
		ErrorBox("Get programing current directory failed!");
		return FALSE;
	}

	HMODULE hUser32=GetModuleHandle("user32.dll");
	if(hUser32)
	{
		MessageBoxTimeout=(MSGBOXAAPI)GetProcAddress(hUser32,"MessageBoxTimeoutA");
		if(MessageBoxTimeout==NULL)
			MessageBox(NULL,"Load MessageBoxTimeoutA fouction failed!","error",MB_OK);
	}

	GetChessTypeResourse();
	hMenu=CreateUserMenu();
	CreateChessTypeMenu(hMenu);
	InitialWithIni();

	return TRUE;
}


