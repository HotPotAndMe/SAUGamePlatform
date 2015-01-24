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
				���淵�������еĹؼ���ֻ�С�name���͡�move����
 */
int getcmd(char *scr,int size,char *cmd)
{
	int pos=-1,i,j,len;
	bool start = true;
	len=strlen(cmd);
	size -= len;
	for(i=0;i<=size;i++)
	{
		if (start)//ƥ����������
		{
			for (j = 0; j<len; j++)
			{
				if (scr[i + j] != cmd[j])
					break;
			}
			if (j == len&&scr[i + j] == ' ')//������ƽ̨���͵�������������壬�ؼ��ֺ���пո�
			{
				pos = i;
				break;
			}
			else//ƥ��ʧ��
			{
				i += j;
				start = false;
			}
		}
		if (scr[i] == ' ' || scr[i] == '\n' || scr[i] == '\t'|| scr[i] == '\0')//�ؼ���ǰ��Ϊ��Щ�ַ�
			start = true;
	}
	return pos;
}

CEngine::CEngine()
{
	status=-1;//δ��������
	linkType=UNNAMEDPIPE;//�����ܵ�����
	indexBuf=0;
}

CEngine::~CEngine()
{
}

void CEngine::SetName(char *nameCmd)
{
	strncpy(name, nameCmd + 5, 255);
}

//ͨ�������ܵ�������������
bool CEngine::LinkEngineWithUnnamed()
{
	status = 0;//��������δ����

	SECURITY_ATTRIBUTES sa;
	sa.nLength=sizeof(sa);
	sa.lpSecurityDescriptor=NULL;//Default security attributes
	sa.bInheritHandle=true;//handle can be inherited

	if(!CreatePipe(&pde.engine_read,&pde.platform_write,&sa,BUFSIZE) ||!CreatePipe(&pde.platform_read,&pde.engine_write,&sa,BUFSIZE))//��������ƽ̨������֮�以��ͨ�ŵ������ܵ�
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
		PROCESS_INFORMATION pi;
		ZeroMemory(&si,sizeof(si));
		si.cb=sizeof(si);
		si.dwFlags=STARTF_USESHOWWINDOW |STARTF_USESTDHANDLES;
		si.wShowWindow=SW_HIDE;
		si.hStdInput=pde.engine_read;
		si.hStdOutput=pde.engine_write;
		si.hStdError=pde.engine_write;
		if(!CreateProcess(path,"",NULL,NULL,true,0,NULL,EngineDir,&si,&pi))//���������
		{
			ErrorBox("CreateProcess failed");
			status = -1;
			return false;
		}
		CloseHandle(pde.engine_read);
		CloseHandle(pde.engine_write);
		WaitForInputIdle(pi.hProcess,INFINITE);
		pde.hEProcess = pi.hProcess;
		CloseHandle(pi.hThread);
		SetCurrentDirectory(gameSet.CurDir);//�ָ���ǰ��Ӧ�ó���Ľ���
		CreateEngineInfoBoard();
	}
	else
	{
		status = -1;
		return false;
	}

	return true;
}

//ͨ�������ܵ���������
bool CEngine::LinkEngineWithNamed()
{
	status = 0;
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
		PROCESS_INFORMATION pi;
		ZeroMemory(&si,sizeof(si));
		si.cb=sizeof(si);
		si.dwFlags=STARTF_USESHOWWINDOW |STARTF_USESTDHANDLES;
		si.wShowWindow=SW_SHOW;
		si.hStdInput=GetStdHandle(STD_INPUT_HANDLE);
		si.hStdOutput=GetStdHandle(STD_OUTPUT_HANDLE);
		si.hStdError=GetStdHandle(STD_ERROR_HANDLE);
		if(!CreateProcess(path,"",NULL,NULL,true,0,NULL,EngineDir,&si,&pi))//���������
		{
			ErrorBox("CreateProcess failed");
			status = -1;
			return false;
		}
		SetCurrentDirectory(gameSet.CurDir);//�ָ���ǰ��Ӧ�ó���Ľ���
		CreateEngineInfoBoard();
	}
	else
	{
		status = -1;
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
	if(status!=-1)//���洦������״̬
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
	if(status!=-1)
		ShowWindow(pde.console_hwnd,nCmdShow);
}

//�������洰��
bool CEngine::CreateEngineInfoBoard()
{
	//Ϊ�˱��ֶԿ���̨����������Ϣ��ʾ��ԭζ�ԣ����򴴽��������һ������̨������ʾ���򣬲�ͨ�������ܵ���������̨д��������Ϣ
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa;
	sa.nLength=sizeof(sa);
	sa.lpSecurityDescriptor=NULL;//Default security attributes
	sa.bInheritHandle=true;//handle can be inherited

	if(!CreatePipe(&pde.console_read,&pde.console_write,&sa,BUFSIZE))//������������ʾ����̨֮�以��ͨ�ŵ������ܵ�
	{
		ErrorBox("CreatePipe failed");
		return false;
	}		
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	si.dwFlags=STARTF_USESHOWWINDOW |STARTF_USESTDHANDLES;
	si.wShowWindow=SW_HIDE;
	si.hStdInput=pde.console_read;
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdError=GetStdHandle(STD_ERROR_HANDLE);
	if(!CreateProcess("bin\\console1.exe",NULL,NULL,NULL,true,0,NULL,gameSet.CurDir,&si,&pi))//���������
	{
		ErrorBox("CreateProcess console1 failed");
		return false;
	}
	CloseHandle(pde.console_read);
	pde.hCProcess = pi.hProcess;
	CloseHandle(pi.hThread);
	WaitForInputIdle(pi.hProcess,INFINITE);
	while((pde.console_hwnd=GetProcessMainWnd(pi.dwProcessId))==NULL)
	{//�����ڽ��̴����������첽�ģ��ӽ��̸ս���ʱ����������ö�ٴ��ڣ���Ҫ�ȴ�
		Sleep(50);
	}
	//���ô��ڱ���Ϊ����·��
	SetWindowText(pde.console_hwnd,path);
	//��ȡ���ڲ˵����
	HMENU hMenu=GetSystemMenu(pde.console_hwnd,NULL);
	//ʹ�رհ�ť��Ч
	EnableMenuItem(hMenu,SC_CLOSE,MF_GRAYED);
	if(gameSet.swEngine==true)
		ShowWindow(pde.console_hwnd,SW_SHOW);
	return true;
}

//ж������
bool CEngine::UnloadEngine()
{
	DWORD k;
	if(status == -1)//���Ƿ���ж��
	{
		MsgBox("Engine has unloaded!","Msg",1500);
		return true;
	}
	if (!GetExitCodeProcess(pde.hEProcess, &k))//��ȡ�˳���
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
				TerminateProcess(pde.hCProcess,0);
				CloseHandle(pde.console_write);
				CloseHandle(pde.platform_write);
				CloseHandle(pde.platform_read);
			}
			status = -1;
			return true;
		}
	}
	if(TerminateProcess(pde.hEProcess,0))//ж�سɹ�
	{
		if(linkType==UNNAMEDPIPE)
		{//�ر������ͬʱ�ر���ʾ���ڣ��ͷ����������ܵ�
			TerminateProcess(pde.hCProcess,0);
			CloseHandle(pde.console_write);
			CloseHandle(pde.platform_write);
			CloseHandle(pde.platform_read);
		}
		MsgBox("UnLoadEngine succeed!","Msg",1500);
	}
	else
	{
		ErrorBox("UnLoadEngine failed");
		return false;
	}
	status = -1;
	return true;
}

//��ȡ������Ϣ
DWORD CEngine::ReadMsg(char *msg,int size)
{	
	DWORD dwRead;		
	HANDLE hFile=NULL;	
	if(linkType==UNNAMEDPIPE)
		hFile=pde.platform_read;
	else if(linkType==NAMEDPIPE)
		hFile=pde.hPipe;
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
		WriteFile(pde.console_write,msg,dwRead,&dwRead,NULL);
	return dwRead;
}

//������д��Ϣ
DWORD CEngine::WriteMsg(char *msg)
{
	DWORD dwWrite;	
	HANDLE hFile=NULL;	
	if(linkType==UNNAMEDPIPE)
		hFile=pde.platform_write;
	else if(linkType==NAMEDPIPE)
		hFile=pde.hPipe;					
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
		WriteFile(pde.console_write,msg,dwWrite,&dwWrite,NULL);
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
		dwSize = ReadMsg(readBuffer + indexBuf, BUFSIZE);
		indexBuf+=dwSize;
		if(dwSize==BUFSIZE)//�� dwSize=BUFSIZE ʱ�������ַ��ضϵĿ���
		{
			pos=getcmd(readBuffer,indexBuf,cmd);
			if(pos==-1)//δ�ҵ������֣����з��ضϴ���
			{
				for(i=indexBuf-1;indexBuf-i<len&&i>=0;i--)
				{
					if (readBuffer[i] == ' ' || readBuffer[i] == '\n' || readBuffer[i] == '\t' || readBuffer[i] == '\0')//��ǰѰ�Ҽ����
						break;
				}
				if(indexBuf-i>=len)//��󴮳��ȴ��ڹؼ��ֳ��ȣ��ز��ǹؼ���
					indexBuf=0;
				else
				{
					memmove(readBuffer, readBuffer + (i + 1), indexBuf - i - 1);
					indexBuf=indexBuf-i-1;
				}
			}
			else
			{
				for(i=pos+len+1;i<indexBuf;i++)
				{
					if (readBuffer[i] == ' ' || readBuffer[i] == '\r' || readBuffer[i] == '\n' || readBuffer[i] == '\t' || readBuffer[i] == '\0')//�����ֺ�����������  ����������������'\n'ǰ����'\r'
					{
						memcpy(CMD,readBuffer+pos,i-pos);
						CMD[i-pos]='\0';
						memmove(readBuffer, readBuffer + (i + 1), indexBuf - i - 1);
						indexBuf=indexBuf-i-1;
						return;
					}
				}
				//�����ֺ������������������ؼ��ֲ�������ȡ������Ϣ
				memmove(readBuffer, readBuffer + pos, indexBuf - pos);
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
					if (readBuffer[i] == ' ' || readBuffer[i] == '\r' || readBuffer[i] == '\n' || readBuffer[i] == '\t' || readBuffer[i] == '\0')//�����ֺ�����������
					{
						memcpy(CMD,readBuffer+pos,i-pos);
						CMD[i-pos]='\0';
						memmove(readBuffer, readBuffer + (i + 1), indexBuf - i - 1);
						indexBuf=indexBuf-i-1;
						return;
					}
				}
				//�����ֺ������������������ؼ��ֲ�������ȡ������Ϣ
				memmove(readBuffer, readBuffer + pos, indexBuf - pos);
				indexBuf=indexBuf-pos;
			}
		}
	}
}

VOID CEngine::CreatePipeAndConnectClient()
{
	if(pde.hPipe!=NULL)return;//����Ѵ��������ܵ��������ظ�����

	char *PipeName="\\\\.\\pipe\\server";
	pde.hPipe=CreateNamedPipe(
		PipeName,
		PIPE_ACCESS_DUPLEX |FILE_FLAG_OVERLAPPED,//open mode
		PIPE_TYPE_MESSAGE |PIPE_READMODE_MESSAGE |PIPE_WAIT,//pipe mode
		PIPE_UNLIMITED_INSTANCES,//the maximum instance which can create the pipe having name specified by PipeName
		BUFSIZE,//the number of byte of the pipe buffer written by server process once
		BUFSIZE,//the number of byte of the pipe buffer read by server process once
		100000,//the time the client process can use to try to connect to server process
		NULL//default security attribute
		);
	if(pde.hPipe==INVALID_HANDLE_VALUE)
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
	if(!ConnectNamedPipe(pde.hPipe,&ol))//If a client process opens the pipe created by server process,the event specified by "ol.hEvent" will be signaled.
	{
		//printf("ConnectNamedPipe failed %d\n",GetLastError());
		//return;
	}
	return;
}
