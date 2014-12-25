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
#include "Network.h"


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
	if(PrepareForApp(hInstance)==FALSE)
		return -1;

	//��ʼ������ģ��
	InitialChessType(hMenu);

	// ע���������࣬������������
	MainWnd=new CMainWnd;
	if(MainWnd->RegisterWnd(hInstance)==FALSE||MainWnd->CreateWnd(NULL,hMenu)==FALSE)
	{
		ErrorBox("Create MainWnd failed!");
		return -1;
	}

	// ע��������񴰿���
	NetWnd = new CNetWnd;
	if(!NetWnd->RegisterWnd(hInstance))
	{
		ErrorBox("Register NetWnd failed");
		return 0;
	}

	// ע������Э��������
	ChatWnd = new CChatWnd;
	if (!ChatWnd->RegisterWnd(hInstance))
	{
		ErrorBox("Register ChatWnd failed");
		return 0;
	}

	// ������ѭ��
	GetAndDispatchMessage();

	return 0;
}

/**
 * GetAndDispatchMessage - ��Ϣѭ��
 */
VOID GetAndDispatchMessage()
{
	HACCEL hAccTable;//���ټ���
	MSG msg;//��Ϣ

	// �����ݼ�
	hAccTable=LoadAccelerators(hInst,MAKEINTRESOURCE(IDR_ACCELERATOR));

#ifdef _DEBUG
	MsgBox("������Ϣѭ���ɹ���","Msg",1500);
#endif

	// ��Ϣѭ��
	while(GetMessage(&msg,NULL,0,0))//����Ϣ������ȡ����Ϣ��������
	{
		if(!TranslateAccelerator(msg.hwnd,hAccTable,&msg))//�ѿ�ݼ���Ϣת�����ַ���Ϣ
		{
			TranslateMessage(&msg);//���û�п�ݼ���Ϣ��ת��������Ϣ
			DispatchMessage(&msg);//�ɷ���Ϣ
		}
	}

	//ɾ�����ټ���
	DestroyAcceleratorTable(hAccTable);
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

	// ���MessageBoxTimeoutϵͳ����
	HMODULE hUser32=GetModuleHandle("user32.dll");
	if(hUser32)
	{
		MessageBoxTimeout=(MSGBOXAAPI)GetProcAddress(hUser32,"MessageBoxTimeoutA");
		if(MessageBoxTimeout==NULL)
			MessageBox(NULL,"Load MessageBoxTimeoutA fouction failed!","error",MB_OK);
	}

	// ��ȡ���������Դ
	if(GetChessTypeResourse()==FALSE)//��ȡ���������Ϣ
		return FALSE;
	hMenu=CreateUserMenu();//�����˵���
	if (hMenu == NULL)
		return FALSE;
	CreateChessTypeMenu(hMenu);//��˵����м�������ѡ��˵�
	InitialWithIni();//ʹ�������ļ���ʼ����������

	// ���������ļ���
	CreateFolder("PrintScr");//������ͼ�ļ���

	return TRUE;
}


