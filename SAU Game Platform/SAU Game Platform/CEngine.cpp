/**
 * file: CEngine.cpp
 * date: 2014/9/10
 * version: 1.2
 * description: SAU_GamePalform ��������������ļ������ݰ�����CEngine��������ĳ�Ա�����Ķ��塣
 */


#include "CEngine.h"


/**
 * getcmd - Ѱ��������
 * @scr:	���黺������ַ
 * @size:	���黺��������
 * @cmd:	�����������׵�ַ
 * @return:	�������ڻ������е�ƫ������-1��ʾ������
 * description����Ϊƽ̨-����ͨ��Э��涨��������ƽ̨���͵������ʽΪ"������+' '+����"������������ƥ�������Ҫ����Ƿ���' '������
				��Ϊ�����������������ַ���ƥ���㷨ʵ�֡����淵�������еĹؼ���ֻ�С�name���͡�move����
 */
int getcmd(char *scr,int size,char *cmd)
{
	int pos=-1,i,j,len;
	len=strlen(cmd);
	for(i=0;i<=size-len;i++)
	{
		for(j=0;j<len;j++)
		{
			if(scr[i+j]!=cmd[j])
				break;
		}
		if(j==len&&scr[i+j]==' ')
		{
			pos=i;
			break;
		}
	}
	return pos;
}

CEngine::CEngine()
{
	linked=false;//δ��������
	linkType=UNNAMEDPIPE;//�����ܵ�����
	indexBuf=0;
}

CEngine::~CEngine()
{
}

//ͨ�������ܵ�������������
bool CEngine::LinkEngineWithUnnamed()
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength=sizeof(sa);
	sa.lpSecurityDescriptor=NULL;//Default security attributes
	sa.bInheritHandle=true;//handle can be inherited

	if(!CreatePipe(&engine_read,&platform_write,&sa,BUFSIZE) ||!CreatePipe(&platform_read,&engine_write,&sa,BUFSIZE))//��������ƽ̨������֮�以��ͨ�ŵ������ܵ�
	{
		ErrorBox("CreatePipe failed");
		return false;
	}

	char Filter[]="(exe files)|*.exe|(all files)|*.*||";//�ļ��˾�	
	CFileDialog FileName(true,NULL,NULL,Filter,NULL,gameSet.EngineInitDir);//�����ļ��Ի�����ʵ��
	if(FileName.DoModal()==IDOK)
	{
		path=FileName.GetFilePath();
		LPCTSTR folder=FileName.GetFolderPath(path);
		char EngineDir[MAX_PATH]={0};
		strcpy(EngineDir,folder);
				
		STARTUPINFO si;
		ZeroMemory(&si,sizeof(si));
		si.cb=sizeof(si);
		si.dwFlags=STARTF_USESHOWWINDOW |STARTF_USESTDHANDLES;
		si.wShowWindow=SW_HIDE;
		si.hStdInput=engine_read;
		si.hStdOutput=engine_write;
		si.hStdError=engine_write;
		if(!CreateProcess(path,"",NULL,NULL,true,0,NULL,EngineDir,&si,&pi))//���������
		{
			ErrorBox("CreateProcess failed");
			return false;
		}
		CloseHandle(engine_read);
		CloseHandle(engine_write);
		WaitForInputIdle(pi.hProcess,INFINITE);
		linked=true;
		SetCurrentDirectory(gameSet.CurDir);//�ָ���ǰ��Ӧ�ó���Ľ���
		CreateEngineInfoBoard();
	}
	else
	{
		return false;
	}

/*	WriteMsg("name?\n");//ѯ����������
	char rMsg[256];
	memset(rMsg,0,sizeof(rMsg));
	GetCommand("name",rMsg);

	strcpy(name,"Name: ");
	strcat(name,rMsg+strlen("name "));//������������� */
	return true;
}

//ͨ�������ܵ���������
bool CEngine::LinkEngineWithNamed()
{
	CreatePipeAndConnectClient();//���������ܵ����ȴ��ͻ�������

	char Filter[]="(exe files)|*.exe|(all files)|*.*||";//�ļ��˾�	
	CFileDialog FileName(true,NULL,NULL,Filter,NULL,gameSet.EngineInitDir);	
	if(FileName.DoModal()==IDOK)
	{
		path=FileName.GetFilePath();
		LPCTSTR folder=FileName.GetFolderPath(path);
		char EngineDir[MAX_PATH]={0};
		strcpy(EngineDir,folder);
				
		STARTUPINFO si;
		ZeroMemory(&si,sizeof(si));
		si.cb=sizeof(si);
		si.dwFlags=STARTF_USESHOWWINDOW |STARTF_USESTDHANDLES;
		si.wShowWindow=SW_SHOW;
		si.hStdInput=GetStdHandle(STD_INPUT_HANDLE);;
		si.hStdOutput=GetStdHandle(STD_OUTPUT_HANDLE);;
		si.hStdError=GetStdHandle(STD_ERROR_HANDLE);
		if(!CreateProcess(path,"",NULL,NULL,true,0,NULL,EngineDir,&si,&pi))//���������
		{
			ErrorBox("CreateProcess failed");
			return false;
		}
		linked=true;
		SetCurrentDirectory(gameSet.CurDir);//�ָ���ǰ��Ӧ�ó���Ľ���
		CreateEngineInfoBoard();
	}
	else
	{
		return false;
	}			

	WaitForSingleObject(ol.hEvent,INFINITE);//�ȴ��ͻ��˳ɹ���������

	WriteMsg("name?\n");//ѯ����������	
	char rMsg[256];
	memset(rMsg,0,sizeof(rMsg));
	GetCommand("name",rMsg);

	strcpy(name,"Name: ");
	strcat(name,rMsg+strlen("name "));//�������������
	return true;
}

//��������
bool CEngine::LoadEngine()
{
	if(linked==true)
	{
		MsgBox("Engine has loaded!","error",3000);
		return false;
	}
	indexBuf=0;
	if(linkType==UNNAMEDPIPE)
	{
		return LinkEngineWithUnnamed();
	}
	else if(linkType==NAMEDPIPE)
	{
		return LinkEngineWithNamed();
	}	
	return false;
}

VOID CEngine::ShowEngineWindow(int nCmdShow)
{
	if(linked)
		ShowWindow(console_hwnd,nCmdShow);
}

//�������洰��
bool CEngine::CreateEngineInfoBoard()
{
	//Ϊ�˱��ֶԿ���̨����������Ϣ��ʾ��ԭζ�ԣ����򴴽��������һ������̨������ʾ���򣬲�ͨ�������ܵ���������̨д��������Ϣ
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;
	sa.nLength=sizeof(sa);
	sa.lpSecurityDescriptor=NULL;//Default security attributes
	sa.bInheritHandle=true;//handle can be inherited

	if(!CreatePipe(&console_read,&console_write,&sa,BUFSIZE))//������������ʾ����̨֮�以��ͨ�ŵ������ܵ�
	{
		ErrorBox("CreatePipe failed");
		return false;
	}		
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	si.dwFlags=STARTF_USESHOWWINDOW |STARTF_USESTDHANDLES;
	si.wShowWindow=SW_HIDE;
	si.hStdInput=console_read;
	si.hStdOutput=stdout;
	si.hStdError=GetStdHandle(STD_ERROR_HANDLE);
	if(!CreateProcess("bin\\console1.exe",NULL,NULL,NULL,true,0,NULL,gameSet.CurDir,&si,&pl))//���������
	{
		ErrorBox("CreateProcess console1 failed");
		return false;
	}
	CloseHandle(console_read);
	WaitForInputIdle(pl.hProcess,INFINITE);
	while((console_hwnd=GetProcessMainWnd(pl.dwProcessId))==NULL)
	{//�����ڽ��̴����������첽�ģ��ӽ��̸ս���ʱ����������ö�ٴ��ڣ���Ҫ�ȴ�
		Sleep(50);
	}
	//���ô��ڱ���Ϊ����·��
	SetWindowText(console_hwnd,path);
	//��ȡ���ڲ˵����
	HMENU hMenu=GetSystemMenu(console_hwnd,NULL);
	//ʹ�رհ�ť��Ч
	EnableMenuItem(hMenu,SC_CLOSE,MF_GRAYED);
	if(gameSet.swEngine==true)
		ShowWindow(console_hwnd,SW_SHOW);
	return true;
}

//ж������
bool CEngine::UnloadEngine()
{
	DWORD k;
	if(linked==false)//���Ƿ���ж��
	{
		MsgBox("Engine has unloaded!","Msg",1500);
		return true;
	}
	if(!GetExitCodeProcess(pi.hProcess,&k))
	{
		ErrorBox("Get exit code failed!");
	}
	else
	{
		if(k!=STILL_ACTIVE)//�쳣�˳�
		{
			MsgBox("Program has been ended for unknown reasons!","error",3000);
			if(linkType==UNNAMEDPIPE)
			{//�ر������ͬʱ�ر���ʾ���ڣ��ͷ����������ܵ�
				TerminateProcess(pl.hProcess,0);
				CloseHandle(console_write);
//				CloseHandle(console_read);
//				CloseHandle(engine_write);	
//				CloseHandle(engine_read);
				CloseHandle(platform_write);
				CloseHandle(platform_read);
			}
			linked=false;
			return true;
		}
	}
	if(TerminateProcess(pi.hProcess,0))//ж�سɹ�
	{
		if(linkType==UNNAMEDPIPE)
		{//�ر������ͬʱ�ر���ʾ���ڣ��ͷ����������ܵ�
			TerminateProcess(pl.hProcess,0);
			CloseHandle(console_write);
//			CloseHandle(console_read);
//			CloseHandle(engine_write);	
//			CloseHandle(engine_read);
//			CloseHandle(platform_write);
//			CloseHandle(platform_read);
		}
		linked=false;
		MsgBox("UnLoadEngine succeed!","Msg",1500);
	}
	else
	{
		ErrorBox("UnLoadEngine failed");
		return false;
	}
	return true;
}

//��ȡ������Ϣ
DWORD CEngine::ReadMsg(char *msg,int size)
{	
	DWORD dwRead;		
	HANDLE hFile=NULL;	
	if(linkType==UNNAMEDPIPE)
		hFile=platform_read;
	else if(linkType==NAMEDPIPE)
		hFile=hPipe;
	else
	{
		MsgBox("���ӳ������ͳ���!","Error",0);
		return 0;
	}

	if(hFile==NULL)
	{
		MsgBox("�����ӳ���֮���ͨ�Źܵ�����!","Error",0);
		return 0;
	}
	if(!ReadFile(hFile,msg,size,&dwRead,NULL))//��ȡ�ܵ���Ϣ
	{
		ErrorBox("ReadMsg failed");
		return 0;
	}
	if(linkType==UNNAMEDPIPE)
		WriteFile(console_write,msg,dwRead,&dwRead,NULL);
	return dwRead;
}

//������д��Ϣ
DWORD CEngine::WriteMsg(char *msg)
{
	DWORD dwWrite,temp;	
	HANDLE hFile=NULL;	
	if(linkType==UNNAMEDPIPE)
		hFile=platform_write;
	else if(linkType==NAMEDPIPE)
		hFile=hPipe;					
	else
	{
		MsgBox("���ӳ������ͳ���!","Error",0);
		return 0;
	}

	if(hFile==NULL)
	{
		MsgBox("�����ӳ���֮���ͨ�Źܵ�����!","Error",0);
		return 0;
	}
	if(!WriteFile(hFile,msg,strlen(msg)+1,&dwWrite,NULL))//���ܵ�д����Ϣ
	{
		ErrorBox("WriteMsg failed");
		return 0;
	}
	if(linkType==UNNAMEDPIPE)
		WriteFile(console_write,msg,dwWrite,&dwWrite,NULL);
	return dwWrite;
}

/**
 * GetCommand - ��ȡ���
 * @cmd:	ϣ��ȡ�������������
 * @CMD��	��������Ļ�����
 */
void CEngine::GetCommand(char *cmd,char *CMD)
{
	int dwSize,pos,len,i;
	len=strlen(cmd);
	while(1)
	{
		dwSize=ReadMsg(readBuffer+BUFSIZE,BUFSIZE);
		memcpy(readBuffer+indexBuf,readBuffer+BUFSIZE,dwSize);
		indexBuf+=dwSize;
		if(dwSize==BUFSIZE)//�� dwSize=BUFSIZE ʱ�������ַ��ضϿ���
		{
			pos=getcmd(readBuffer,indexBuf,cmd);
			if(pos==-1)//δ�ҵ������֣����з��ضϴ���
			{
				for(i=indexBuf-1;indexBuf-i<len&&i>=0;i--)
				{
					if(readBuffer[i]==' '||readBuffer[i]=='\n'||readBuffer[i]=='\0')//Ѱ�Ҽ����
						break;
				}
				if(indexBuf-i>=len)//��󴮳��ȴ��ڹؼ��ֳ��ȣ��ز��ǹؼ���
					indexBuf=0;
				else
				{
					memcpy(readBuffer,readBuffer+(i+1),indexBuf-i+1);
					indexBuf=indexBuf-i+1;
				}
			}
			else
			{
				for(i=pos+len+1;i<indexBuf;i++)
				{
					if(readBuffer[i]==' '||readBuffer[i]=='\r'||readBuffer[i]=='\n'||readBuffer[i]=='\0')//�����ֺ�����������  ����������������'\n'ǰ����'\r'
					{
						memcpy(CMD,readBuffer+pos,i-pos);
						CMD[i-pos]='\0';
						memcpy(readBuffer,readBuffer+(i+1),indexBuf-i-1);
						indexBuf=indexBuf-i-1;
						return;
					}
				}
				//�����ֺ������������������ؼ��ֲ�������ȡ������Ϣ
				memcpy(readBuffer,readBuffer+pos,indexBuf-pos);
				indexBuf=indexBuf-pos;
			}
		}
		else
		{
			pos=getcmd(readBuffer,indexBuf,cmd);
			if(pos==-1)//δ�ҵ������֣�������
				indexBuf=0;
			else
			{
				for(i=pos+len+1;i<indexBuf;i++)
				{
					if(readBuffer[i]==' '||readBuffer[i]=='\r'||readBuffer[i]=='\n'||readBuffer[i]=='\0')//�����ֺ�����������
					{
						memcpy(CMD,readBuffer+pos,i-pos);
						CMD[i-pos]='\0';
						memcpy(readBuffer,readBuffer+(i+1),indexBuf-i-1);
						indexBuf=indexBuf-i-1;
						return;
					}
				}
				//�����ֺ������������������ؼ��ֲ�������ȡ������Ϣ
				memcpy(readBuffer,readBuffer+pos,indexBuf-pos);
				indexBuf=indexBuf-pos;
			}
		}
	}
}

VOID CEngine::CreatePipeAndConnectClient()
{
	if(hPipe!=NULL)return;//����Ѵ��������ܵ��������ظ�����

	char *PipeName="\\\\.\\pipe\\server";
	hPipe=CreateNamedPipe(
		PipeName,
		PIPE_ACCESS_DUPLEX |FILE_FLAG_OVERLAPPED,//open mode
		PIPE_TYPE_MESSAGE |PIPE_READMODE_MESSAGE |PIPE_WAIT,//pipe mode
		PIPE_UNLIMITED_INSTANCES,//the maximum instance which can create the pipe having name specified by PipeName
		BUFSIZE,//the number of byte of the pipe buffer written by server process once
		BUFSIZE,//the number of byte of the pipe buffer read by server process once
		100000,//the time the client process can use to try to connect to server process
		NULL//default security attribute
		);
	if(hPipe==INVALID_HANDLE_VALUE)
	{
		printf("CreateNamedPipe failed with %d\n",GetLastError());
		return;
	}
	ol.hEvent=CreateEvent(
		NULL,//default security attribute
		TRUE,//reset event state to nonsignaled automatically.
		FALSE,//Initial state is nonsignaled.
		NULL//The event is without a name.
		);
	if(!ConnectNamedPipe(hPipe,&ol))//If a client process opens the pipe created by server process,the event specified by "ol.hEvent" will be signaled.
	{
		//printf("ConnectNamedPipe failed %d\n",GetLastError());
		//return;
	}
	return;
}
