#include "Network.h"
#include "Control.h"
#include "GameType.h"


char buffer[MTU]; //������
char FAR * lpBuffer = buffer;

ServerInfo HOSTINFO={{0,2048},NULL},CLIENTINFO={{0,2048},NULL},SERVERINFO={{0,2047},NULL};
MessageQueue SendBuffer,ReceiveBuffer;
HANDLE SendProc,ReceiveProc;

ClientInfo clientInfo={0};

LPHOSTENT phost;
struct hostent far *hostaddr;//������������Ϣ
struct hostent hostnm; 

int NetWork=0;//��������־: 0.δ����������� 1.��Ե� 2.�ͻ���-������
int GameMode_2;//��������ģʽ���
bool NetMode=false;//��Ե�����ģʽ�н�ɫ��־: true.��Ϊ������ false.��Ϊ�ͻ���
bool ConnectMode=false;//�ͻ���-����������ģʽ�����ӱ�־: true.�����ӵ������� false.δ���ӵ�������
int index=0;


BOOL CALLBACK NetSetWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
VOID InitNetSet(HWND hWnd);
VOID AddNetLVWnd(ClientInfo *clientInfo);
BOOL ConnectClient(HWND hWnd);
BOOL ConnectServer(HWND hWnd);
inline VOID InsertSendQueue(HWND hWnd,SOCKET ns,LPCTSTR message,INT size);
DWORD WINAPI SendInfo(LPVOID lpThreadParameter);
inline void ReceiveInfo(HWND hWnd,WPARAM wParam,LPARAM lParam);
inline VOID InsertReceiveQueue(HWND hWnd,SOCKET ns,LPCTSTR message,INT size);
DWORD WINAPI DealReceiveInfo(LPVOID lpThreadParameter);

BOOL Client(HWND hWnd,WPARAM wParam,LPARAM lParam);
BOOL Connect(HWND hWnd,WPARAM wParam,LPARAM lParam);
BOOL Server(HWND hWnd,WPARAM wParam,LPARAM lParam);
BOOL Accept(HWND hWnd,WPARAM wParam,LPARAM lParam);


int NetError(char *message)
{
	char buf[256];
	int error;
	error = WSAGetLastError();
	sprintf(buf, "%s\nerror:%d\n", message, error);
	MessageBox(NULL, (LPSTR)buf, "Warning", MB_ICONEXCLAMATION);
	return error;
}


/**
 * NetWndProc - ������񴰿ڻص�����
 */
LRESULT CALLBACK NetWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE://����WinSockģ��
		WORD wMajorVersion, wMinorVersion; //WinSock֧�ְ汾
		LPWSADATA lpmyWSAData; 
		WORD VersionReqd; 
		int ret;

		wMajorVersion = MAJOR_VERSION; 
		wMinorVersion = MINOR_VERSION; 
		VersionReqd = WSA_MAKEWORD(wMajorVersion,wMinorVersion);
		lpmyWSAData = (LPWSADATA)malloc(sizeof(WSADATA)); 
		if(lpmyWSAData==NULL)
		{
			MsgBox("�ڴ��������!����WinSockʧ��","Error",0);
			break;
		}
		//����WinSockģ��
		if ((ret=WSAStartup(VersionReqd, lpmyWSAData))!= 0) 
			NetError("WSAStartup() failed\n"); 

		//��ȡ����IP
		gethostname(buffer,sizeof(buffer));
		phost=gethostbyname(buffer);
		HOSTINFO.ad.ip=ntohl(((struct in_addr *)phost->h_addr)->S_un.S_addr);

		//��ʼ���������ģ��
		NetWork=1;
		NetMode=false;
		ConnectMode=false;
		ChatWnd->hNet=NetWnd->hNet=CreateDialog(hInst,MAKEINTRESOURCE(IDD_NET),NULL,NetSetWndProc);//�����������öԻ���

		SendProc=CreateThread(NULL,NULL,SendInfo,NULL,NULL,0);
		ReceiveProc=CreateThread(NULL,NULL,DealReceiveInfo,NULL,NULL,0);
		break;
	case WM_SOCKET://�׽�����Ϣ
		if(WSAGETSELECTERROR(lParam))
		{
			NetError("WSAGETSELECTERROR!");
			if(closesocket((SOCKET)wParam)==SOCKET_ERROR)
				NetError("closesocket error!");
			return FALSE;
		}
		else
		{
			switch(WSAGETSELECTEVENT(lParam))
			{
			case FD_READ://������Ϣ
				ReceiveInfo(hWnd,wParam,lParam);
				break;
			case FD_WRITE:
				break;
			case FD_ACCEPT:
				Accept(hWnd,wParam,lParam);
				break;
			case FD_CONNECT:
				Connect(hWnd,wParam,lParam);
				break;
			case FD_CLOSE:
				if(WSAAsyncSelect((SOCKET)wParam,hWnd,0,0) == SOCKET_ERROR) 
					NetError("WSAAsyncSelect Failed.\n");
				if (wParam == SERVERINFO.s)//�������׽��ֶϿ�����
				{
					SendMessage(NetWnd->hNet, IDB_DISCONNECT_SERVER, NULL, NULL);
				}
				else if(wParam==CLIENTINFO.s)//�ͻ����׽��ֶϿ�����
				{
					closesocket(wParam);
					CLIENTINFO.s=NULL;
					MsgBox("Opponent has disconnect!","Msg",2000);
					if(ChatWnd->hWnd!=NULL)
						SendMessage(NetWnd->hWnd,GM_DESTROYCHAT,NULL,NULL);
				}
				else
					closesocket(wParam);
				break;
			}
		}
		break;
	case WM_DESTROY:
		NetWork=0;//������������־
		DestroyWindow(NetWnd->hNet);//�����������öԻ���
		NetWnd->hNet=NULL;
		// �رմ򿪵��׽���
		if(SERVERINFO.s!=NULL)
			closesocket(SERVERINFO.s);
		if(CLIENTINFO.s!=NULL)
			closesocket(CLIENTINFO.s);
		if(HOSTINFO.s!=NULL)
			closesocket(HOSTINFO.s);
		WSACleanup();//�ر�WinSockģ��
		break;
	case GM_CREATECHAT:
		ChatWnd->CreateWnd();
		break;
	case GM_DESTROYCHAT:
		DestroyWindow(ChatWnd->hWnd);
		break;
	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return 0;
}

/**
 * NetShell - ������ǣ�������׽��ַ��͡����յ���Ϣ�Ĵ���
 */
VOID NetShell(HWND hWnd,SOCKET ns,LPSTR command,INT size,BOOL send_tag)
{
	char message[MTU+5];
	switch(send_tag)
	{
	case 0://�������������Ϣ
		switch(command[1])
		{
		case 'm':
			ChatWnd->AppendMessage(&command[4],1);
			break;
		case 'e':
			ZeroMemory(message,sizeof(message));
			game.MoveStep(&command[3]);//������һ�˵�ƽ̨��������ά���ŷ�����ȷ��
			break;
		case 'h':
			switch(command[4])
			{
			case 'R':
				EnableWindow(GetDlgItem(ChatWnd->hWnd,IDB_CHAT_RS),TRUE);
				break;
			case 'S':
				game.StartGame();
				break;
			case 'Q':
				if(command[5]=='B')
				{
					EnableWindow(GetDlgItem(ChatWnd->hWnd,IDB_CHAT_APPLY_WHT),FALSE);
					if(MessageBox(ChatWnd->hWnd,"Opponent request is BLACK,whether or\nnot to accept the request?","Apply",MB_YESNO)==IDYES)
					{
						GameMode_2=1;
						EnableWindow(GetDlgItem(ChatWnd->hWnd,IDB_CHAT_APPLY_BLC),TRUE);
						NetShell(hWnd,ns,"-AB",sizeof("-AB"),3);
					}
					else
					{
						GameMode_2=0;
						EnableWindow(GetDlgItem(ChatWnd->hWnd,IDB_CHAT_APPLY_WHT),TRUE);
						NetShell(hWnd,ns,"-DW",sizeof("-DW"),3);
					}
				}
				else
				{
					EnableWindow(GetDlgItem(ChatWnd->hWnd,IDB_CHAT_APPLY_BLC),FALSE);
					if(MessageBox(ChatWnd->hWnd,"Opponent request is WHITE,whether or\nnot to accept the request?","Apply",MB_YESNO)==IDYES)
					{
						GameMode_2=0;
						EnableWindow(GetDlgItem(ChatWnd->hWnd,IDB_CHAT_APPLY_WHT),TRUE);
						NetShell(hWnd,ns,"-AW",sizeof("-AW"),3);
					}
					else
					{
						GameMode_2=1;
						EnableWindow(GetDlgItem(ChatWnd->hWnd,IDB_CHAT_APPLY_BLC),TRUE);
						NetShell(hWnd,ns,"-DB",sizeof("-DB"),3);
					}
				}
				break;
			case 'A':
				if(command[5]=='B')
				{
					GameMode_2=0;
					EnableWindow(GetDlgItem(ChatWnd->hWnd,IDB_CHAT_APPLY_WHT),TRUE);
				}
				else
				{
					GameMode_2=1;
					EnableWindow(GetDlgItem(ChatWnd->hWnd,IDB_CHAT_APPLY_BLC),TRUE);
				}
				MsgBox("Opponent accept your request","Answer",1500);
				break;
			case 'D':
				if(command[5]=='B')
				{
					GameMode_2=1;
					EnableWindow(GetDlgItem(ChatWnd->hWnd,IDB_CHAT_APPLY_BLC),TRUE);
				}
				else
				{
					GameMode_2=0;
					EnableWindow(GetDlgItem(ChatWnd->hWnd,IDB_CHAT_APPLY_WHT),TRUE);
				}
				MsgBox("Opponent refused your request","Answer",1500);
				break;
			}
			break;
		case 's'://������Ϣ��������Ӧ��
			switch(command[4])
			{
			case 'S'://��ʼ���������б�
				EnableWindow(GetDlgItem(NetWnd->hNet,IDB_SERVER_REFRESH),FALSE);
				index=0;
				ListView_DeleteAllItems(GetDlgItem(NetWnd->hNet,IDL_SERVER_LIST));//���֮ǰ�����
				break;
			case 'L'://���������б���
				sscanf(command,"-s -L %s %s %s %s",clientInfo.IP,clientInfo.chessType,clientInfo.state,clientInfo.code);
				AddNetLVWnd(&clientInfo);
				break;
			case 'E'://ֹͣ���������б�
				EnableWindow(GetDlgItem(NetWnd->hNet,IDB_SERVER_REFRESH),TRUE);
				break;
			case 'Q'://��ѯ�����б�
				break;
			case 'C'://��������
				break;
			case 'A':
				if(command[6]=='H')
					GameMode_2=0;
				else
					GameMode_2=1;
				SendMessage(NetWnd->hWnd,GM_CREATECHAT,NULL,NULL);
				break;
			case 'D'://�Ͽ�����
				MsgBox("Opponent has disconnect!","Msg",2000);
				if(ChatWnd->hWnd!=NULL)
					SendMessage(NetWnd->hWnd,GM_DESTROYCHAT,NULL,NULL);
				EnableWindow(GetDlgItem(NetWnd->hNet,ConnectMode==false?IDB_CONNECT:IDB_CONNECT_SERVER),TRUE);
				break;
			case 'T'://��������
				sprintf(message,"-T %s",chessType[chesstype].chessStr);
				NetShell(hWnd,ns,message,strlen(message)+1,4);
				break;
/*			case 'G'://�Ŵ��㷨
				switch(command[7])
				{
				case 'Q'://����ͻ�����Ϊ�Ŵ��㷨�ͻ���
					NetShell(hWnd,ns,"-G -A",sizeof("-G -A"),4);
					initialLineTypeTable(preTable);
					initialLineInfoTable(linetypeInfo);
					break;
				case 'T'://��������
					NetShell(hWnd,ns,"-G -S",sizeof("-G -S"),4);
					break;
				case 'S'://�����׼����
					memcpy(&standNum,command+9,sizeof(int));
					standParam=(Simple*)malloc(sizeof(Simple)*standNum);
					if(standParam==NULL)
						ErrorBox("standParam=NULL!");
					for(int i=0;i<standNum;i++)
					{
						memcpy(standParam[i].param,command+9+sizeof(int)+i*sizeof(unsigned int)*GENELEN,sizeof(unsigned int)*GENELEN);
					}
					NetShell(hWnd,ns,"-G -I",sizeof("-G -I"),4);
					break;
				case 'I'://��������
					memcpy(&simpleNum,command+9,sizeof(int));
					simpleParam=(Simple*)malloc(sizeof(Simple)*simpleNum);
					for(int i=0;i<simpleNum;i++)
					{
						memcpy(simpleParam[i].param,command+9+sizeof(int)+i*sizeof(unsigned int)*GENELEN,sizeof(unsigned int)*GENELEN);
						simpleParam[i].value=0;
					}
					NetShell(hWnd,ns,"-G -R",sizeof("-G -R"),4);
					TestParam(ns);
					break;
				case 'N':
					memcpy(&simpleNum,command+9,sizeof(int));
					simpleParam=(Simple*)malloc(sizeof(Simple)*simpleNum);
					for(int i=0;i<simpleNum;i++)
					{
						memcpy(simpleParam[i].param,command+9+sizeof(int)+i*sizeof(unsigned int)*GENELEN,sizeof(unsigned int)*GENELEN);
						simpleParam[i].value=0;
					}
					CalculateStand(ns);
					break;
				}
				break;  */
			}
			break;
		}
		break;
	case 1://����������Ϣ
		sprintf(message,"-m ");
		memcpy(message+3,command,size);
		InsertSendQueue(hWnd,ns,message,size+3);
		break;
	case 2://���Ͷ�����Ϣ
		sprintf(message,"-e ");
		memcpy(message+3,command,size);
		InsertSendQueue(hWnd,ns,message,size+3);
		break;
	case 3://����������Ϣ
		sprintf(message,"-h ");
		memcpy(message+3,command,size);
		InsertSendQueue(hWnd,ns,message,size+3);
		break;
	case 4://���ͷ�������Ϣ
		sprintf(message,"-s ");
		memcpy(message+3,command,size);
		InsertSendQueue(hWnd,ns,message,size+3);
		break;
	}
}

/**
 * UpdateCheck - �����������ô���ѡ������
 * @hWnd:	�������ô��ھ��
 */
VOID UpdateCheck(HWND hWnd)
{
	if(NetWork==1)
	{
		EnableWindow(GetDlgItem(hWnd,IDR_AS_CLIENT),TRUE);
		EnableWindow(GetDlgItem(hWnd,IDR_AS_SERVER),TRUE);
		SendMessage(GetDlgItem(hWnd,IDR_CONNECT_P2P),BM_SETCHECK,(WPARAM)BST_CHECKED,(LPARAM)0);
		if(NetMode)
		{
			EnableWindow(GetDlgItem(hWnd,IDB_CONNECT),FALSE);
			SendMessage(GetDlgItem(hWnd,IDR_AS_CLIENT),BM_SETCHECK,(WPARAM)BST_UNCHECKED,(LPARAM)0);
			SendMessage(GetDlgItem(hWnd,IDR_AS_SERVER),BM_SETCHECK,(WPARAM)BST_CHECKED,(LPARAM)0);
		}
		else
		{
			EnableWindow(GetDlgItem(hWnd,IDB_CONNECT),TRUE);
			SendMessage(GetDlgItem(hWnd,IDR_AS_CLIENT),BM_SETCHECK,(WPARAM)BST_CHECKED,(LPARAM)0);
			SendMessage(GetDlgItem(hWnd,IDR_AS_SERVER),BM_SETCHECK,(WPARAM)BST_UNCHECKED,(LPARAM)0);
		}
	}
	else
	{
		EnableWindow(GetDlgItem(hWnd,IDR_AS_CLIENT),FALSE);
		EnableWindow(GetDlgItem(hWnd,IDR_AS_SERVER),FALSE);
		EnableWindow(GetDlgItem(hWnd,IDB_CONNECT),FALSE);
		SendMessage(GetDlgItem(hWnd,IDR_CONNECT_P2P),BM_SETCHECK,(WPARAM)BST_UNCHECKED,(LPARAM)0);
	}
	if(NetWork==2)
	{
		EnableWindow(GetDlgItem(hWnd,IDB_CONNECT_SERVER),TRUE);
		SendMessage(GetDlgItem(hWnd,IDR_CONNECT_SERVER),BM_SETCHECK,(WPARAM)BST_CHECKED,(LPARAM)0);
		if(ConnectMode)
		{
			SetWindowText(GetDlgItem(hWnd,IDB_CONNECT_SERVER),"�����ն�");
			EnableWindow(GetDlgItem(hWnd,IDB_DISCONNECT_SERVER),TRUE);
			EnableWindow(GetDlgItem(hWnd,IDB_SERVER_REFRESH),TRUE);
		}
		else
		{
			SetWindowText(GetDlgItem(hWnd,IDB_CONNECT_SERVER),"���ӷ�����");
			EnableWindow(GetDlgItem(hWnd,IDB_DISCONNECT_SERVER),FALSE);
			EnableWindow(GetDlgItem(hWnd,IDB_SERVER_REFRESH),FALSE);
		}
	}
	else
	{
		EnableWindow(GetDlgItem(hWnd,IDB_CONNECT_SERVER),FALSE);
		EnableWindow(GetDlgItem(hWnd,IDB_DISCONNECT_SERVER),FALSE);
		EnableWindow(GetDlgItem(hWnd,IDB_SERVER_REFRESH),FALSE);
		SendMessage(GetDlgItem(hWnd,IDR_CONNECT_SERVER),BM_SETCHECK,(WPARAM)BST_UNCHECKED,(LPARAM)0);
		SetWindowText(GetDlgItem(hWnd,IDB_CONNECT_SERVER),"���ӷ�����");
	}
}

BOOL CALLBACK NetSetWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	int ID;
	char command[30];
	switch(uMsg)
	{
	case WM_INITDIALOG:	//��ʼ��
		InitNetSet(hWnd);
		break;
	case WM_COMMAND:
		ID=LOWORD(wParam);
		switch(ID)
		{
		case IDR_CONNECT_P2P://��Ե�ģʽ
			if(NetWork==1)
				break;
			NetWork=1;//�ӿͻ���-������ģʽ�л�����Ե�ģʽ����ʼ״̬Ϊ�ͻ�����
			NetMode=false;
			if(ConnectMode)//�رտͻ���-������ģʽ�п��ܵ������ӷ�����״̬
				SendMessage(hWnd,IDB_DISCONNECT_SERVER,NULL,NULL);//�Ͽ������������
			else
				UpdateCheck(hWnd);
			break;
		case IDR_CONNECT_SERVER://�ͻ���-������ģʽ
			if(NetWork==2)
				break;
			NetWork=2;//��Ե�ģʽ�л����ӿͻ���-������ģʽ����ʼ״̬�ͻ��˲����ӷ�����
			if(HOSTINFO.s!=NULL)//�رյ�Ե�ģʽ�п��ܿ����ķ�������״̬
			{
				closesocket(HOSTINFO.s);
				HOSTINFO.s=NULL;
			}
			UpdateCheck(hWnd);
			break;
		case IDR_AS_CLIENT://��Ե�����ģʽ����Ϊ�ͻ���
			if(NetMode)
			{
				NetMode=false;
				if(HOSTINFO.s!=NULL)//�رյ�Ե�ģʽ�п��ܿ����ķ�������״̬
				{
					closesocket(HOSTINFO.s);
					HOSTINFO.s=NULL;
				}
			}
			UpdateCheck(hWnd);
			break;
		case IDR_AS_SERVER://��Ե�����ģʽ����Ϊ������
			if(NetMode==true)
				break;
			EnableWindow(hWnd,FALSE);
			if(Server(hWnd,wParam,lParam))
				NetMode=true;
			EnableWindow(hWnd,TRUE);
			UpdateCheck(hWnd);
			break;
		case IDB_CONNECT://��Ե�ģʽ������ƽ̨����
			if(ConnectClient(hWnd))
			{
				EnableWindow((HWND)lParam,FALSE);
			}
			break;
		case IDB_CONNECT_SERVER://���ӻ������ϵķ�����
			if(ConnectMode==false)
			{
				if(ConnectServer(hWnd))
					ConnectMode=true;
				UpdateCheck(hWnd);
			}
			else
			{
				ID=ListView_GetSelectedCount(GetDlgItem(hWnd,IDL_SERVER_LIST));
				if(ID==1)
				{
					EnableWindow((HWND)lParam,FALSE);
					index=ListView_GetSelectionMark(GetDlgItem(hWnd,IDL_SERVER_LIST));
					sprintf(command,"-C ");
					ListView_GetItemText(GetDlgItem(hWnd,IDL_SERVER_LIST),index,3,&command[3],10);
					NetShell(hWnd,SERVERINFO.s,command,strlen(command)+1,4);
				}
				else if(ID==0)
				{
					MsgBox("��ѡ����Ҫ���ӵ��ն�","error",1500);
				}
				else
				{
					MsgBox("�벻Ҫѡ�����ն�","error",1500);
				}
			}
			break;
		case IDB_DISCONNECT_SERVER://�Ͽ��������������
			if(SERVERINFO.s!=NULL)
			{
				closesocket(SERVERINFO.s);
				SERVERINFO.s=NULL;
			}
			ConnectMode=false;
			UpdateCheck(hWnd);
			break;
		case IDB_SERVER_REFRESH://ˢ�·����б�
			NetShell(hWnd,SERVERINFO.s,"-Q",sizeof("-Q"),4);
			break;
		}
		break;
	case WM_CLOSE:
		ShowWindow(hWnd,SW_HIDE);
		break;
	default:
		break;
	}
	return FALSE;
}

VOID InitNetSet(HWND hWnd)
{
	HWND hNetLVWnd;
	char PORT[10];

	SendMessage(GetDlgItem(hWnd,IDT_HOST_IPADDR),IPM_SETADDRESS,0,HOSTINFO.ad.ip);
	itoa(HOSTINFO.ad.port,PORT,10);
	SetText(GetDlgItem(hWnd,IDT_HOST_PORT),PORT);
	SendMessage(GetDlgItem(hWnd,IDT_DESTINATION_IPADDR),IPM_SETADDRESS,0,CLIENTINFO.ad.ip);
	itoa(CLIENTINFO.ad.port,PORT,10);
	SetText(GetDlgItem(hWnd,IDT_DESTINATION_PORT),PORT);
	SendMessage(GetDlgItem(hWnd,IDT_SERVER_IPADDR),IPM_SETADDRESS,0,SERVERINFO.ad.ip);
	itoa(SERVERINFO.ad.port,PORT,10);
	SetText(GetDlgItem(hWnd,IDT_SERVER_PORT),PORT);

	UpdateCheck(hWnd);

	/*�����б���ͼ*/
	hNetLVWnd = CreateWindow (
		WC_LISTVIEW, // List View������		
		NULL,		
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS,		
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hWnd,
		(HMENU)IDL_SERVER_LIST,
		hInst, NULL);	// �޲˵�����ͼ��
	if (hNetLVWnd == NULL)
	{
		MessageBox(NULL,"CreateListView failed","error",MB_OK);
		return;
	}
	RECT rt,rt1,rt2;
	GetWindowRect(GetDlgItem(hWnd,IDC_SERVER_INFO),&rt);
	GetWindowRect(GetDlgItem(hWnd,IDT_SERVER_PORT),&rt1);
	GetWindowRect(GetDlgItem(hWnd,IDB_CONNECT_SERVER),&rt2);
	MapWindowPoints(NULL,hWnd,(LPPOINT)&rt,2);
	MapWindowPoints(NULL,hWnd,(LPPOINT)&rt1,2);
	MapWindowPoints(NULL,hWnd,(LPPOINT)&rt2,2);
	if(!SetWindowPos(hNetLVWnd,HWND_TOP,rt.left,rt.bottom+3,rt1.right-rt.left,rt2.top-rt.bottom-6,SWP_SHOWWINDOW))
		ErrorBox("Set window position failed!");

	/*��ʼ��ͼ���б�*/
	HIMAGELIST himl=NULL; 
	ListView_SetImageList(hNetLVWnd, himl, LVSIL_SMALL);
	ListView_SetExtendedListViewStyle(hNetLVWnd,LVS_EX_FULLROWSELECT|LVS_EX_SINGLEROW);

	/*��ʼ����*/	
	LVCOLUMN lvc;
	DWORD i;
	// ��������
	LPSTR ColNames[] = {"�ͻ���IP","����","״̬","ʶ����"};
	// LVCOLUMN����Ч�ĳ�Ա
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	// ���LVCOLUMN�ṹ	
	lvc.iImage = 1;
	lvc.fmt = LVCFMT_LEFT;	// �������
	// ǰ����
	for(i=0; i<4; i++)
	{
		//	���������ı�
		if(i==0)lvc.cx=120;//��IP��һ�����ÿ��
		else lvc.cx=100;
		lvc.pszText = ColNames[i];
		lvc.iSubItem = i;
		// ����ListView_InsertColumn�������
		if (ListView_InsertColumn(hNetLVWnd,i,&lvc) == -1)
		{
			MessageBox(NULL,"ListView_InsertColumn failed","error",MB_OK);
			return;
		}
	}
	return;
}

VOID AddNetLVWnd(ClientInfo *clientInfo)
{
	HWND hWnd=GetDlgItem(NetWnd->hNet,IDL_SERVER_LIST);
	LVITEM lvI;	
	ZeroMemory (&lvI,sizeof(lvI));
	// ��Ч����
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	// ���LVITEM
	lvI.state = 1;
	lvI.stateMask = 0;
	lvI.iImage = 0;
	lvI.iSubItem = 0;
	lvI.iItem = index;				
	lvI.pszText = clientInfo->IP; 
	lvI.cchTextMax = lstrlen(lvI.pszText)+1;
	// ������
	if(ListView_InsertItem(hWnd, &lvI) == -1)
	{
		MessageBox(NULL,"ListView_InsertItem failed","errror",MB_OK);
		return;
	}			
	// ����������ı���0 based
	ListView_SetItemText(hWnd, index, 1,clientInfo->chessType);
	ListView_SetItemText(hWnd, index, 2,clientInfo->state);
	ListView_SetItemText(hWnd, index, 3,clientInfo->code);
	index++;
	return;
}

/**
 * ConnectClient - ������һ��ƽ̨����
 */
BOOL ConnectClient(HWND hWnd)
{
	char PORT[10];
	u_long addr_ip_n;
	SendMessage(GetDlgItem(hWnd,IDT_DESTINATION_IPADDR),IPM_GETADDRESS,0,(LPARAM)&CLIENTINFO.ad.ip);
	addr_ip_n=htonl(CLIENTINFO.ad.ip);
	if(gethostbyaddr((char*)&addr_ip_n,4,AF_INET)==NULL)
	{
		MessageBox(NULL,"ipѰַ����","error",MB_OK);
		return FALSE;
	}
	GetText(GetDlgItem(hWnd,IDT_DESTINATION_PORT),PORT,9);
	if(PORT[0]==0)
	{
		MsgBox("�����˿ں�","error",3000);
		return FALSE;
	}
	CLIENTINFO.ad.port=atoi(PORT);
	return Client(hWnd,CLIENTINFO.ad.port,CLIENTINFO.ad.ip);
}

/**
 * ConnectServer - ���ӻ������ϵķ�����
 */
BOOL ConnectServer(HWND hWnd)
{
	char PORT[10];
	u_long addr_ip_n;
	SendMessage(GetDlgItem(hWnd,IDT_SERVER_IPADDR),IPM_GETADDRESS,0,(LPARAM)&SERVERINFO.ad.ip);
	addr_ip_n=htonl(SERVERINFO.ad.ip);
	if(gethostbyaddr((char*)&addr_ip_n,4,AF_INET)==NULL)
	{
		MessageBox(NULL,"ipѰַ����","error",MB_OK);
		return FALSE;
	}
	GetText(GetDlgItem(hWnd,IDT_SERVER_PORT),PORT,9);
	if(PORT[0]==0)
	{
		MsgBox("�����˿ں�","error",3000);
		return FALSE;
	}
	SERVERINFO.ad.port=atoi(PORT);
	return Client(hWnd,SERVERINFO.ad.port,SERVERINFO.ad.ip);
}

/**
 * ChatWndProc - ����Э�����ڻص�����
 */
LRESULT CALLBACK ChatWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	int ID;
	char text[10];
	switch(uMsg)
	{
	case WM_CREATE:
		ChatWnd->CreateCtrl(hWnd);
		break;
	case WM_PAINT:
		ChatWnd->OnPaint(wParam,lParam);
		break;
	case WM_ERASEBKGND:
		break;
	case WM_COMMAND:
		ID=LOWORD(wParam);
		switch(ID)
		{
		case IDB_CHAT_RS:
			GetWindowText((HWND)lParam,text,10);
			if(text[0]=='R')
			{
				EnableWindow((HWND)lParam,FALSE);
				NetShell(hWnd,ConnectMode?SERVERINFO.s:CLIENTINFO.s,"-R",sizeof("-R"),3);
			}
			else
			{
				EnableWindow((HWND)lParam,FALSE);
				NetShell(hWnd,ConnectMode?SERVERINFO.s:CLIENTINFO.s,"-S",sizeof("-S"),3);
				game.StartGame();
			}
			break;
		case IDB_CHAT_DISCONNECT:
			if(ConnectMode)
				NetShell(hWnd,SERVERINFO.s,"-D",sizeof("-D"),4);
			else
				if(CLIENTINFO.s!=NULL)
				{
					closesocket(CLIENTINFO.s);
					CLIENTINFO.s=NULL;
				}
			DestroyWindow(hWnd);
			EnableWindow(GetDlgItem(NetWnd->hNet,IDB_CONNECT_SERVER),TRUE);
			break;
		case IDB_CHAT_APPLY_BLC:
			EnableWindow((HWND)lParam,FALSE);
			NetShell(hWnd,ConnectMode?SERVERINFO.s:CLIENTINFO.s,"-QB",sizeof("-QB"),3);
			break;
		case IDB_CHAT_APPLY_WHT:
			EnableWindow((HWND)lParam,FALSE);
			NetShell(hWnd,ConnectMode?SERVERINFO.s:CLIENTINFO.s,"-QW",sizeof("-QW"),3);
			break;
		case IDB_CHAT_SEND:
			GetText(GetDlgItem(hWnd,IDE_CHAT_SEND),buffer,MTU);
			ChatWnd->AppendMessage(buffer,0);
			NetShell(hWnd,ConnectMode?SERVERINFO.s:CLIENTINFO.s,buffer,strlen(buffer)+1,1);
			SetText(GetDlgItem(hWnd,IDE_CHAT_SEND),"");
			break;
		}
		break;
	case WM_SIZE:
		ChatWnd->AdjustCtrlPos(hWnd);
		break;
	case WM_CLOSE:
		break;
	case WM_DESTROY:
		ChatWnd->OnDestroy(wParam,lParam);
		break;
	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return 0;
}

inline VOID InsertSendQueue(HWND hWnd,SOCKET ns,LPCTSTR message,INT size)
{
	MessageBuf *Sendbuf;
	Sendbuf=(MessageBuf*)malloc(sizeof(MessageBuf));
	if(Sendbuf==NULL)
	{
		MsgBox("�ڴ�����ʧ��!\nλ�ã�winsock��Ϣ���Ͷ���\n�����������Ϣʧ�ܣ���Ϣ��ʧ","Error",0);
		return;
	}
	Sendbuf->ns=ns;
	memcpy(Sendbuf->MessageBuffer,message,size);
	Sendbuf->size=size;
	while((SendBuffer.tail+1)%100==SendBuffer.head)
		Sleep(10);
	SendBuffer.MB[SendBuffer.tail]=Sendbuf;
	SendBuffer.tail=(SendBuffer.tail+1)%100;
	return;
}
DWORD WINAPI SendInfo(LPVOID lpThreadParameter)
{
	int l,len;
	SendBuffer.head=0;
	SendBuffer.tail=0;
	while(NetWork!=0)
	{
		if(SendBuffer.head==SendBuffer.tail)
			Sleep(100);
		else
		{
			len=SendBuffer.MB[SendBuffer.head]->size;
			l=0;
			do{
				l+=send(SendBuffer.MB[SendBuffer.head]->ns,&SendBuffer.MB[SendBuffer.head]->MessageBuffer[l],len,0);
				if(l==SOCKET_ERROR)
				{
					NetError("Send error!");
					continue;
				}
				len-=l;
			}while(len>0);
			free(SendBuffer.MB[SendBuffer.head]);
			SendBuffer.MB[SendBuffer.head]=NULL;
			SendBuffer.head=(SendBuffer.head+1)%100;
			Sleep(10);
		}
	}
	return 0;
}

void ReceiveInfo(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	char Msg[MTU];
	int size=recv((SOCKET)wParam,Msg,MTU,0);
	InsertReceiveQueue(hWnd,(SOCKET)wParam,Msg,size);
}

inline VOID InsertReceiveQueue(HWND hWnd,SOCKET ns,LPCTSTR message,INT size)
{
	MessageBuf *Receivebuf;
	Receivebuf=(MessageBuf*)malloc(sizeof(MessageBuf));
	if(Receivebuf==NULL)
	{
		MsgBox("�ڴ�����ʧ��!\nλ�ã�winsock��Ϣ��ȡ\n�����������Ϣ��ʧ","Error",0);
		return;
	}
	Receivebuf->ns=ns;
	memcpy(Receivebuf->MessageBuffer,message,size);
	Receivebuf->size=size;
	while((ReceiveBuffer.tail+1)%100==ReceiveBuffer.head)
		Sleep(10);
	ReceiveBuffer.MB[ReceiveBuffer.tail]=Receivebuf;
	ReceiveBuffer.tail=(ReceiveBuffer.tail+1)%100;
	return;
}
DWORD WINAPI DealReceiveInfo(LPVOID lpThreadParameter)
{
	ReceiveBuffer.head=0;
	ReceiveBuffer.tail=0;
	while(NetWork!=0)
	{
		if(ReceiveBuffer.head==ReceiveBuffer.tail)
			Sleep(100);
		else
		{
			NetShell(NULL,ReceiveBuffer.MB[ReceiveBuffer.head]->ns,ReceiveBuffer.MB[ReceiveBuffer.head]->MessageBuffer,ReceiveBuffer.MB[ReceiveBuffer.head]->size,0);
			free(ReceiveBuffer.MB[ReceiveBuffer.head]);
			ReceiveBuffer.MB[ReceiveBuffer.head]=NULL;
			ReceiveBuffer.head=(ReceiveBuffer.head+1)%100;
		}
	}
	return 0;
}


//�ͻ��˲���
//=====================================================================================

/**
 * Client - �ͻ��˳�����������Ŀ���׽���
 */
BOOL Client(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	SOCKET s;
	struct sockaddr_in dst_addr;//Ŀ���׽�����Ϣ
	ZeroMemory(&dst_addr,sizeof(dst_addr)); 
	dst_addr.sin_addr.S_un.S_addr=htonl((u_long)lParam); 
	dst_addr.sin_family=AF_INET; 
	dst_addr.sin_port=htons((u_short)wParam); 
	if ((s = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET) 
	{ 
		NetError("Socket Failed"); 
		return FALSE; 
	} 
	if(WSAAsyncSelect(s,NetWnd->hWnd,WM_SOCKET,FD_CONNECT|FD_CLOSE) == SOCKET_ERROR)//�첽�����¼������ӡ��ر�
	{
		NetError("WSAAsyncSelect failed");
		return FALSE; 
	}
	connect(s,(struct sockaddr FAR *)&dst_addr, sizeof(dst_addr));
	return TRUE;

}

/**
 * Connect - ���Ӵ���
 */
BOOL Connect(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	if(WSAAsyncSelect((SOCKET)wParam,NetWnd->hWnd,WM_SOCKET,FD_WRITE|FD_READ|FD_CLOSE) == SOCKET_ERROR)//�첽�����¼�������д���ر�
	{
		NetError("WSAAsyncSelect failed");
		return FALSE; 
	}
	MsgBox("Connect successed!","Msg",2000);
	if(NetWork==2)
	{
		SERVERINFO.s=(SOCKET)wParam;
		NetShell(hWnd,(SOCKET)wParam,"-Q",sizeof("-Q"),4);
	}
	else if(NetWork==1)
	{
		CLIENTINFO.s=(SOCKET)wParam;
		NetShell(hWnd,(SOCKET)wParam,"-A H",sizeof("-A -H"),4);
		NetShell(hWnd,NULL,"-s -A G",sizeof("-s -A G"),0);
	}
	else
		closesocket((SOCKET)wParam);
	return TRUE;
}


//����������
//===============================================================================================

/**
 * Server - ��������������������������
 */
BOOL Server(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	SOCKET s;
	struct sockaddr_in server;//���������׽�����Ϣ
	ZeroMemory(&server,sizeof(server)); 
	char PORT[10];
	GetText(GetDlgItem(hWnd,IDT_HOST_PORT),PORT,9);
	if(PORT[0]==0)
	{
		MsgBox("�����˿ں�","error",3000);
		return FALSE;
	}
	HOSTINFO.ad.port=atoi(PORT);
	server.sin_family = AF_INET;//Э����
	server.sin_addr.s_addr = INADDR_ANY;//��ַ
	server.sin_port=htons(HOSTINFO.ad.port);//���˿ں�
	if((s=socket(AF_INET,SOCK_STREAM, 0))==INVALID_SOCKET) 
	{ 
		NetError("Socket() failed\n"); 
		return FALSE; 
	} 
	if(bind(s,(struct sockaddr *)&server, sizeof(server))==SOCKET_ERROR)//�����׽���
	{ 
		NetError("Bind() failed!"); 
		return FALSE; 
	}
	if(WSAAsyncSelect(s,NetWnd->hWnd,WM_SOCKET,FD_ACCEPT|FD_CLOSE) == SOCKET_ERROR)//�첽�����¼������ܡ��ر�
	{
		NetError("WSAAsyncSelect failed");
		return FALSE; 
	}
	if(listen(s,1)!=0)//�����׽���
	{ 
		NetError("Listen() failed!"); 
		return FALSE; 
	}
	HOSTINFO.s=s;
	MsgBox("As server successed!","Msg",1500);
	return TRUE;
}

/*
 * Accept - ����������
 */
BOOL Accept(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	SOCKET s;
	struct sockaddr_in client;//�ͻ����׽�����Ϣ
	int namelen=sizeof(client);
	ZeroMemory(&client,sizeof(client));
	s=accept((SOCKET)wParam,(struct sockaddr *)&client,&namelen);//��������
	if(s==INVALID_SOCKET)
	{
		NetError("accept() failed!");
		return FALSE;
	}
	else
	{
		if(CLIENTINFO.s!=NULL)//��������ӣ���ܾ�����
		{
			closesocket(s);
			return FALSE;
		}
		else
			MsgBox("Connect Successed!","Msg",1500);
	}
	if (WSAAsyncSelect(s,hWnd,WM_SOCKET,FD_READ|FD_WRITE|FD_CLOSE) == SOCKET_ERROR)//�첽�����¼�������д���ر�
	{
		NetError("WSAAsyncSelect failed");
		return (FALSE); 
	}
//	NetShell(hWnd,"-TQ",s,4);
	CLIENTINFO.ad.ip=ntohl(client.sin_addr.S_un.S_addr);
	CLIENTINFO.ad.port=ntohs(client.sin_port);
	CLIENTINFO.s=s;
	return TRUE;
}



