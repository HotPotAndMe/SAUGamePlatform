/**
 * file: CEngine.h
 * date: 2014/9/10
 * version: 1.2
 * description: SAU_GamePalform �������������ͷ�ļ������ݰ�����CEngine��Ķ��弰���Ա������������
 */


#ifndef CENGINE_H_GAMEPLATFORM
#define CENGINE_H_GAMEPLATFORM

#include "Game Platform.h"

#define BUFSIZE 256
#define FULLBUFSIZE 512
#define UNNAMEDPIPE 0
#define NAMEDPIPE 1


typedef struct _PROCESSDESCRIBE
{
	HANDLE engine_read, platform_write;//�����ƽ̨д�����ܵ�
	HANDLE platform_read, engine_write;//����дƽ̨�������ܵ�
	HANDLE hPipe;//�����ܵ�
	HANDLE hEProcess;
	HWND console_hwnd;//������Ϣ���ھ��
	HANDLE console_write, console_read;//������Ϣ���ڹܵ�
	HANDLE hCProcess;
}PROCESSDESCRIBE;

class CEngine
{
public:
	CEngine();
	~CEngine();

	void SetType(int type){linkType=type;}
	bool GetLoaded(){return linked;}
	void SetName(char *nameCmd);
	char* GetName(){return name;}
	//��������
	bool LoadEngine();
	//ж������
	bool UnloadEngine();
	//��ȡ������Ϣ
	DWORD ReadMsg(char *msg,int size);
	//������д����Ϣ
	DWORD WriteMsg(char *msg);
	void GetCommand(char *cmd,char *CMD);
	VOID ShowEngineWindow(int nCmdShow);

	VOID CreatePipeAndConnectClient();

private:
	int linkType;//��������
	bool linked;//�Ƿ�����������
	char name[256];//������
	LPCTSTR path;//�������·��
	OVERLAPPED ol;

	char readBuffer[FULLBUFSIZE];//��������Ϣ������
	char indexBuf;

	PROCESSDESCRIBE pde;

	//ͨ�������ܵ�������������
	bool LinkEngineWithUnnamed();
	//ͨ�������ܵ���������
	bool LinkEngineWithNamed();
	//������Ϣ��ʾ����
	bool CreateEngineInfoBoard();
};
#endif