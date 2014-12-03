/**
 * file: 
 * date: 
 * version: 
 * description: 
 */

#include "CMainWnd.h"
#include "Menu.h"
#include "Function.h"
#include "GameType.h"


const char EName[]="Name:Human";
CMainWnd *MainWnd;//�����ڶ���ָ��


LRESULT CALLBACK MainWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return MainWnd->WndProc(hWnd,uMsg,wParam,lParam);
}


//���캯��
CMainWnd::CMainWnd()
{
	ClsName="GamePlatform"; //��������
	WndName="Game Platform";//�������
	strcpy(strBlcName, EName);//�ڷ�������
	strcpy(strWhtName, EName);//�׷�������
	strcpy(strBlcTime, "Time:00:00");
	strcpy(strWhtTime,"Time:00:00");
	strcpy(StepHis, "Step History\r\n"
					"------------------------------------\r\n");
}

//��������
CMainWnd::~CMainWnd()
{
}

//ע�ᴰ����
BOOL CMainWnd::RegisterWnd(HINSTANCE hInst)
{		
	WNDCLASS wc;//������

	wc.hInstance=hInst;//����ʵ��
	wc.lpszClassName=ClsName;//��������
	wc.lpfnWndProc=(WNDPROC)MainWndProc;//����ص������ĵ�ַ
	wc.hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON));//ͼ��
	wc.lpszMenuName=NULL;//�˵���
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);//ָ��
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);//������ˢ
	wc.style=CS_HREDRAW |CS_VREDRAW |CS_DBLCLKS;//����֧��˫���¼���ˮƽ����ֱ�����С�ı�ʱ�ػ洰��
	wc.cbWndExtra=0;
	wc.cbClsExtra=0;
	if(RegisterClass(&wc)==NULL)
	{
		ErrorBox("RegisterMainWindowClass failed");
		return FALSE;
	}
	return TRUE;
}

//��������
BOOL CMainWnd::CreateWnd(HWND hParentWnd,HMENU hMenu)
{
	DWORD WndStyle;//���ڷ��

	/*������ʽ*/
	WndStyle=WS_VISIBLE |
			WS_OVERLAPPED |	
			WS_CAPTION |
			WS_SYSMENU |		
			WS_MAXIMIZEBOX |
			WS_MINIMIZEBOX |		
			WS_SIZEBOX |
			WS_CLIPCHILDREN |//Excludes the area occupied by child windows when drawing occurs within the parent window.���Ӵ���֮�䲻����Ϊ�ص������ƻ���
			WS_CLIPSIBLINGS ;//Excludes the area occupied by child windows when drawing occurs within another child window.�Ӵ���֮�䲻����Ϊ�ص������ƻ���	

	hWnd=CreateWindow(
		ClsName,//Class name
		WndName,//Window name		
		WndStyle,//WndStyle
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		(HWND)hParentWnd,//No parent window		
		hMenu,//Main menu
		hInst,//Application instance
		NULL//Do not create MDI client window
		);	
	if(hWnd==NULL)
	{		
		ErrorBox("CreateMainWindow failed");		
		return FALSE;
	}

	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);	
	return TRUE;
}

BOOL CMainWnd::CreateCtrl(HWND hWnd)
{
	HWND hButtonBlcLoad;//�ڷ��������水ť���ھ��
	HWND hButtonWhtLoad;//�׷��������水ť���ھ��
	HWND hButtonBlcUnload;//�ڷ�ж�����水ť���ھ��
	HWND hButtonWhtUnload;//�׷�ж�����水ť���ھ��
	HWND hButtonBlcOk;//�ڷ�ȷ���з���ť���ھ��
	HWND hButtonWhtOk;//�׷�ȷ���з���ť���ھ��
	HWND hButtonBlcCancel;//�ڷ�ȡ���з���ť���ھ��
	HWND hButtonWhtCancel;//�׷�ȡ���з���ť���ھ��

	//���� ��ʷ�з� �ı���
	hEditHis=CreateWindow(
		"EDIT",
		NULL,
		WS_VISIBLE |WS_CHILD |WS_HSCROLL |WS_VSCROLL |WS_BORDER |
		// �ı�����ʽ
		ES_LEFT |ES_MULTILINE |ES_AUTOHSCROLL |ES_AUTOVSCROLL|ES_READONLY|ES_NOHIDESEL,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDE_STEP_HIS,
		hInst,
		NULL
		);
	if(hEditHis==NULL)
	{
		ErrorBox("CreateEditWnd Failed");
		return false;
	}
	SetText(GetDlgItem(hWnd,IDE_STEP_HIS),StepHis);	
	ShowWindow(hEditHis,SW_SHOWNORMAL);
	UpdateWindow(hEditHis);

	//�����ڷ� �������� ��ť
	hButtonBlcLoad=CreateWindow(
		"BUTTON",
		"Load",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_ENGINE_LOAD_BLC,
		hInst,
		NULL
		);
	if(hButtonBlcLoad==NULL)
	{
		ErrorBox("CreateLoadBlcWnd Failed");
		return false;
	}
	ShowWindow(hButtonBlcLoad,SW_SHOWNORMAL);
	UpdateWindow(hButtonBlcLoad);

	//�����ڷ� ж������ ��ť
	hButtonBlcUnload=CreateWindow(
		"BUTTON",
		"Unload",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_ENGINE_UNLOAD_BLC,
		hInst,
		NULL
		);
	if(hButtonBlcUnload==NULL)
	{
		ErrorBox("CreateUnloadBlcWnd Failed");
		return false;
	}
	ShowWindow(hButtonBlcUnload,SW_SHOWNORMAL);
	UpdateWindow(hButtonBlcUnload);

	//�����ڷ� ȷ���з� ��ť
	hButtonBlcOk=CreateWindow(
		"BUTTON",
		"Ok",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_CONTROL_OK_BLC,
		hInst,
		NULL
		);
	if(hButtonBlcOk==NULL)
	{
		ErrorBox("CreateOkBlcWnd Failed");
		return false;
	}
	ShowWindow(hButtonBlcOk,SW_SHOWNORMAL);
	UpdateWindow(hButtonBlcOk);
	EnableWindow(hButtonBlcOk, FALSE);

	//�����ڷ� ȡ���з� ��ť
	hButtonBlcCancel=CreateWindow(
		"BUTTON",
		"Cancel",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_CONTROL_CANCEL_BLC,
		hInst,
		NULL
		);
	if(hButtonBlcCancel==NULL)
	{
		ErrorBox("CreateCancelBlcWnd Failed");
		return false;
	}
	ShowWindow(hButtonBlcCancel,SW_SHOWNORMAL);
	UpdateWindow(hButtonBlcCancel);
	EnableWindow(hButtonBlcCancel, FALSE);

	//�����׷� �������� ��ť
	hButtonWhtLoad=CreateWindow(
		"BUTTON",
		"Load",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_ENGINE_LOAD_WHT,
		hInst,
		NULL
		);
	if(hButtonWhtLoad==NULL)
	{
		ErrorBox("CreateLoadWhtWnd Failed");
		return false;
	}
	ShowWindow(hButtonWhtLoad,SW_SHOWNORMAL);
	UpdateWindow(hButtonWhtLoad);

	//�����׷� ж������ ��ť
	hButtonWhtUnload=CreateWindow(
		"BUTTON",
		"Unload",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_ENGINE_UNLOAD_WHT,
		hInst,
		NULL
		);
	if(hButtonWhtUnload==NULL)
	{
		ErrorBox("CreateUnloadWhtWnd Failed");
		return false;
	}
	ShowWindow(hButtonWhtUnload,SW_SHOWNORMAL);
	UpdateWindow(hButtonWhtUnload);

	//�����׷� ȷ���з� ��ť
	hButtonWhtOk=CreateWindow(
		"BUTTON",
		"Ok",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_CONTROL_OK_WHT,
		hInst,
		NULL
		);
	if(hButtonWhtOk==NULL)
	{
		ErrorBox("CreateOkWhtWnd Failed");
		return false;
	}
	ShowWindow(hButtonWhtOk,SW_SHOWNORMAL);
	UpdateWindow(hButtonWhtOk);
	EnableWindow(hButtonWhtOk, FALSE);

	//�����׷� ȡ���з� ��ť
	hButtonWhtCancel=CreateWindow(
		"BUTTON",
		"Cancel",
		WS_VISIBLE |WS_CHILD,
		CW_USEDEFAULT,0,//Default position
		CW_USEDEFAULT,0,//Default size
		hWnd,
		(HMENU)IDB_CONTROL_CANCEL_WHT,
		hInst,
		NULL
		);
	if(hButtonWhtCancel==NULL)
	{
		ErrorBox("CreateCancelBlcWnd Failed");
		return false;
	}
	ShowWindow(hButtonWhtCancel,SW_SHOWNORMAL);
	UpdateWindow(hButtonWhtCancel);
	EnableWindow(hButtonWhtCancel, FALSE);

	return true;
}

LRESULT CMainWnd::WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) 
{
	//�������ں��ȷ���WM_CREATE��Ϣ�����ؼ����ٷ���WM_ERASEBKGND��Ϣ����������Ȼ����WM_SIZE��Ϣ�������ڴ�С������WM_PAINT��Ϣ�����ػ�
	switch(uMsg)
	{	
	case WM_CREATE:
		OnCreate(hWnd,wParam,lParam);
		break;
	case WM_PAINT:
		OnPaint(wParam,lParam);//������Ϣ		
		break;
	case WM_ERASEBKGND:
		return OnEraseBkgnd(wParam,lParam);
	case WM_SIZE:
		OnSize(wParam, lParam);//��С�ı���Ϣ
		break;
	case WM_MOVE:
		GetWindowRect(hWnd, &rtWindow);//��ȡ����������Ĵ��ھ���
		break;
	case WM_NOTIFY:
		OnNotify(wParam,lParam);//�Ӵ���֪ͨ��Ϣ
		break;
	case WM_COMMAND:
		OnCommand(wParam,lParam);//�˵����߰�ť�ؼ���Ϣ
		break;
	case WM_MENUSELECT:	
		OnMenuSelect(wParam,lParam);	//�˵�ѡ����Ϣ			
		break;
	case WM_LBUTTONDOWN:		
		OnLButtonDown(wParam,lParam);//���������Ϣ
		break;
	case WM_LBUTTONDBLCLK:
		OnLButtonDbClick(wParam,lParam);//���˫����Ϣ
		break;
	case WM_RBUTTONDOWN:
		OnRButtonDown(wParam,lParam);//�Ҽ�������Ϣ
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam,lParam);//����ƶ���Ϣ
		break;
	case WM_KEYDOWN:		
		OnKeyDown(wParam,lParam);//���̰�����Ϣ
		break;	
	case WM_TIMER:
		OnTimer(wParam,lParam);//��ʱ����Ϣ
		break;		
	case WM_CLOSE:
		OnClose(wParam,lParam);	//�رմ�����Ϣ
		break;//�û�����رհ�ťʱ������Ҫϵͳ������
	case WM_DESTROY:		
		OnDestroy(wParam,lParam);		//�ƻ�������Ϣ
		break;

	//�Զ�����Ϣ
	case GM_NAME://����������
		SetName((char*)wParam, (int)lParam);
		break;
	case GM_SHOWSTEP://��ʾ�ŷ���Ϣ��������֧��ģ�鷢�ͣ�
		AppendStepHis((char*)wParam);
		Function::SaveBoardShot();
		Function::SaveChessManual((char*)wParam);
		break;
	case GM_WINLOSE:
		Function::GameStop();
		ShowWiner((int)lParam);
		break;

	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);//������ȫ����Ϣ����������Ϣͳһ��ϵͳ����		
	}
	return 0;
}

//��Ӧ���崴��
VOID CMainWnd::OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	CreateCtrl(hWnd);
	CT_InitModule(hWnd,chessType[chesstype].LibPath);
	return;
}

//��Ӧ��ͼ
VOID CMainWnd::OnPaint(WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;	
	HDC hDC;//����DC
	HDC hMemDC;//���洰��DC
	HBITMAP hBitmap,hOldBitmap;

	hDC=BeginPaint(hWnd,&ps);

	hMemDC=CreateCompatibleDC(hDC);//��������DC
	hBitmap=CreateCompatibleBitmap(hDC,rtClient.right-rtClient.left,rtClient.bottom-rtClient.top);//����λͼ
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);//��λͼѡ�����DC

	FillBkAndBoard(hMemDC);//����������汳��
	DrawCtrlBoard(hMemDC);//���Ʋ������
	CT_DrawBoard(hMemDC);//��������

	BitBlt(hDC,rtClient.left,rtClient.top,rtClient.right-rtClient.left,rtClient.bottom-rtClient.top,hMemDC,rtClient.left,rtClient.top,SRCCOPY);//�ü���DCͼ����´���ͼ��
	
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);

	EndPaint(hWnd,&ps);

	return;
}

//��Ӧ��������
BOOL CMainWnd::OnEraseBkgnd(WPARAM wParam,LPARAM lParam)
{
	return TRUE;
}

//��Ӧ���ڴ�С�ı�
VOID CMainWnd::OnSize(WPARAM wParam,LPARAM lParam)
{
	int cx, cy;
	cx = LOWORD(lParam);
	cy = HIWORD(lParam);
	if (cx < 650 || cy < 400)
	{
		if (cy >= 400)
			MoveWindow(hWnd, rtWindow.left, rtWindow.top, 650 + rtWindow.right - rtWindow.left - rtClient.right, rtWindow.bottom - rtWindow.top, true);
		else if (cx >= 600)
			MoveWindow(hWnd, rtWindow.left, rtWindow.top, rtWindow.right - rtWindow.left, 400 + rtWindow.bottom - rtWindow.top - rtClient.bottom, true);
		else
			MoveWindow(hWnd, rtWindow.left, rtWindow.top, 650 + rtWindow.right - rtWindow.left - rtClient.right, 400 + rtWindow.bottom - rtWindow.top - rtClient.bottom, true);
		return;
	}
	//	StatusBarOnSize();//�������С�ı������һ��״̬��
	if(!AdjustWndPos())
	{		
		return;
	}
	CT_OnSize(rtBoard);
	return;
}

VOID CMainWnd::OnNotify(WPARAM wParam,LPARAM lParam)
{
	return;
}

//��Ӧ������Ϣ
VOID CMainWnd::OnCommand(WPARAM wParam,LPARAM lParam)
{
	int ID=LOWORD(wParam);	//�˵���ID���߰�ťID
	FUNCTION function;
	map<int,FUNCTION>::iterator iter;
	switch(ID)
	{
	case IDB_CONTROL_OK_BLC:
	case IDB_CONTROL_OK_WHT:
		UpdateTime();
		Function::OkMove();
		break;
	case IDB_CONTROL_CANCEL_BLC:
	case IDB_CONTROL_CANCEL_WHT:
		Function::CancelMove();
		break;
	case IDB_ENGINE_LOAD_BLC:
		Function::LoadBlackEngine();
		break;
	case IDB_ENGINE_LOAD_WHT:
		Function::LoadWhiteEngine();
		break;
	case IDB_ENGINE_UNLOAD_BLC:
		Function::UnloadBlackEngine();
		break;
	case IDB_ENGINE_UNLOAD_WHT:
		Function::UnloadWhiteEngine();
		break;
	default:
		if(menuFunction.count(ID))
		{
			iter=menuFunction.find(ID);
			iter->second();
		}
		else if (ID >= CHESSTYPE&&ID < CHESSTYPE + chessNum)
			SetChessType(ID - CHESSTYPE, hMenu);
		break;
	}
	return;
}

//��Ӧ�˵���ѡ���޸�״̬����ʾ��
VOID CMainWnd::OnMenuSelect(WPARAM wParam,LPARAM lParam)
{
	int ID=LOWORD(wParam);	//�˵���ID���߰�ťID
//	UINT wIDs[]={MH_BASE_0,MH_BASE_1,0,0};
//		MenuHelp(WM_MENUSELECT,wParam,lParam,hCurMenu,languagePack[language].languageRE,StatusBar.hWnd,wIDs);
	return;
}

VOID CMainWnd::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{	
	int x,y;
	x=LOWORD(lParam);
	y=HIWORD(lParam);//��ȡ�������̴���ͻ��������꣬������ѡ����ִ�ж�Ӧ���ֵ����¼�������
	if(InsideRect(&rtBoard,x,y))//������Ϊ��Ч����
	{
		Function::MoveStep(x, y);
	}
	return;
}

VOID CMainWnd::OnLButtonDbClick(WPARAM wParam,LPARAM lParam)
{
	return;
}

VOID CMainWnd::OnRButtonDown(WPARAM wParam,LPARAM lParam)
{
	return;
}

VOID CMainWnd::OnMouseMove(WPARAM wParam,LPARAM lParam)
{                                               
	int x,y;
	x=LOWORD(lParam);
	y=HIWORD(lParam);
	
	return;
}

VOID CMainWnd::OnKeyDown(WPARAM wParam,LPARAM lParam)
{
	return;
}

VOID CMainWnd::OnTimer(WPARAM wParam,LPARAM lParam)
{	
	if (timer.UpdateTime(strBlcTime, strWhtTime) == 0)
		InvalidateRect(hWnd, &rtBlcTime, FALSE);
	else
		InvalidateRect(hWnd, &rtWhtTime, FALSE);
	return;
}

VOID CMainWnd::OnClose(WPARAM wParam,LPARAM lParam)
{
	if (Function::CheckEngineLoad())
	{
		MsgBox("The engine is load,unload it and then quit.", "error", 0);
		return;
	}
	RecordUserOper();
	DestroyWindow(hWnd);//���ٴ���
	return;
}

VOID CMainWnd::OnDestroy(WPARAM wParam,LPARAM lParam)
{
	DestroyMenu(hCurMenu);//���ٲ˵���Դ
	PostQuitMessage(0);//����ƻ����壬���˳����߳�
	return;
}

//��Ӧ������Ϣ����������ʱ�����������Ƹ���һ��
VOID CMainWnd::OnName(WPARAM wParam,LPARAM lParam)
{
	return;
}

//��ȡ�����ڿͻ���RECT(������״̬��)
VOID CMainWnd::GetClientRectEx(RECT* rt)
{	
	GetClientRect(hWnd,rt);	
	return; 
}

//���������Ӵ���λ��
BOOL CMainWnd::AdjustWndPos()
{	
	if(hWnd==NULL)
	{
		return FALSE;
	}	
	GetWindowRect(hWnd,&rtWindow);//��ȡ����������Ĵ��ھ���
	GetClientRectEx(&rtClient);//��ȡ������ͻ����ľ���

	/*����������ʾ������������ͻ�����λ��*/
	rtBoardPart.left=rtClient.left;
	rtBoardPart.top=rtClient.top;
	rtBoardPart.right=(int)(rtClient.right*RATIO);
	rtBoardPart.bottom=rtClient.bottom;
	GetBkAndBoardRT();//�����������ͼ��λ��

	/*������Ϣ����ʾ������������ͻ�����λ��*/
	rtInfoPart.left=rtBoardPart.right+1;
	rtInfoPart.top=rtClient.top;
	rtInfoPart.right=rtClient.right;
	rtInfoPart.bottom=rtClient.bottom;
	AdjustCtrlPos();//����������ͼ��λ��

	return TRUE;
}

/**
 * GetBkAndBoardRT - �����������ͼ��Ԫ��λ��
 */
VOID CMainWnd::GetBkAndBoardRT()
{
	//����ͼƬλ��
	rtBackground.left=rtBoardPart.left;
	rtBackground.right=rtBoardPart.right;
	rtBackground.top=rtBoardPart.top;
	rtBackground.bottom=rtBoardPart.bottom;

	//�������̱߳�
	BoardSide=(rtBoardPart.right>rtBoardPart.bottom? rtBoardPart.bottom:rtBoardPart.right)+1;
	BoardSide-=(MARGIN*2);

	//��������λ��
	rtBoard.left=(int)((rtBoardPart.right-BoardSide+1)/2);
	rtBoard.right=rtBoard.left+BoardSide-1;		
	rtBoard.top=(int)((rtBoardPart.bottom-BoardSide+1)/2);
	rtBoard.bottom=rtBoard.top+BoardSide-1;	

	//����ڷ�����ͼƬλ��
	rtBlcPer.right=rtBoard.left-rtBoard.left/15;
	rtBlcPer.left=rtBlcPer.right-rtBoard.left*2/3;
	rtBlcPer.top=rtBoard.top+BoardSide*0.2;
	rtBlcPer.bottom=rtBlcPer.top+(rtBlcPer.right-rtBlcPer.left+1)*1.681-1;//�߶ȸ��ݿ�ȱ��ֻƽ����
	if((rtBlcPer.bottom-rtBlcPer.top+1)>BoardSide*0.5)//�߶ȳ�������޶�
	{
		rtBlcPer.bottom=rtBlcPer.top+BoardSide*0.5-1;
		rtBlcPer.left=rtBlcPer.right-(rtBlcPer.bottom-rtBlcPer.top+1)*0.681+1;//��ȸ��ݸ߶ȱ��ֻƽ����
	}

	//����׷�����ͼƬλ�ã���ڷ�����ͼƬλ�öԳƣ�
	rtWhtPer.left=rtBoard.right+(rtClient.right-rtBoard.right)/15;
	rtWhtPer.right=rtWhtPer.left+(rtBlcPer.right-rtBlcPer.left);
	rtWhtPer.top=rtBlcPer.top;
	rtWhtPer.bottom=rtBlcPer.bottom;
}

/**
 * AdjustCtrlPos - ���㲢�����ؼ�λ��
 */
BOOL CMainWnd::AdjustCtrlPos()
{	
	//**����ͼ��Ԫ��λ��

	//��׼ƫ��
	edge.cx=(int)((rtInfoPart.right-rtInfoPart.left)*0.05);
	edge.cy=(int)((rtInfoPart.bottom-rtInfoPart.top)*0.05/3);
	//�ڷ������
	rtSBlc.left=rtInfoPart.left;			rtSBlc.right=rtInfoPart.right;
	rtSBlc.top=rtInfoPart.top;				rtSBlc.bottom=rtSBlc.top+(int)(rtInfoPart.bottom/3);
	//�ڷ���������̬��
	rtBlcName.left=rtSBlc.left+2*edge.cx;	rtBlcName.right=rtSBlc.right-2*edge.cx;
	rtBlcName.top=rtSBlc.top+2*edge.cy;		rtBlcName.bottom=rtBlcName.top+(int)((rtSBlc.bottom-rtSBlc.top-7*edge.cy)/4);
	//�ڷ���ʱ��̬��
	rtBlcTime.left=rtSBlc.left+2*edge.cx;	rtBlcTime.right=rtSBlc.right-2*edge.cx;
	rtBlcTime.top=rtBlcName.bottom+edge.cy;	rtBlcTime.bottom=rtBlcTime.top+(int)((rtSBlc.bottom-rtSBlc.top-7*edge.cy)/4);
	//�׷������
	rtSWht.left=rtInfoPart.left;			rtSWht.right=rtInfoPart.right;
	rtSWht.top=rtSBlc.bottom+1;				rtSWht.bottom=rtSWht.top+(int)((rtInfoPart.bottom)/3);
	//�׷���������̬��
	rtWhtName.left=rtSWht.left+2*edge.cx;	rtWhtName.right=rtSWht.right-2*edge.cx;
	rtWhtName.top=rtSWht.top+2*edge.cy;		rtWhtName.bottom=rtWhtName.top+(int)((rtSWht.bottom-rtSWht.top-7*edge.cy)/4);
	//�׷���ʱ��̬��
	rtWhtTime.left=rtSWht.left+2*edge.cx;	rtWhtTime.right=rtSWht.right-2*edge.cx;
	rtWhtTime.top=rtWhtName.bottom+edge.cy;	rtWhtTime.bottom=rtWhtTime.top+(int)((rtSWht.bottom-rtSWht.top-7*edge.cy)/4);	


	//**�����ռ�λ��

	//������ʷ�з��ı���λ��
	if(!SetWindowPos(
		hEditHis,
		HWND_TOP,
		rtSWht.left+edge.cx,
		rtSWht.bottom+edge.cy,
		rtSWht.right-rtSWht.left-2*edge.cx,
		rtSWht.bottom-rtSWht.top-2*edge.cy,
		SWP_SHOWWINDOW
		))
		return false;

	//���úڷ��������水ťλ��
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_ENGINE_LOAD_BLC),
		HWND_TOP,
		rtSBlc.left+2*edge.cx,
		rtBlcTime.bottom+edge.cy,
		(int)((rtSBlc.right-rtSBlc.left-5*edge.cx)/2),
		(int)((rtSBlc.bottom-rtSBlc.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//���úڷ�ж�����水ťλ��
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_ENGINE_UNLOAD_BLC),
		HWND_TOP,
		rtSBlc.right-(int)((rtSBlc.right-rtSBlc.left-edge.cx)/2),
		rtBlcTime.bottom+edge.cy,
		(int)((rtSBlc.right-rtSBlc.left-5*edge.cx)/2),
		(int)((rtSBlc.bottom-rtSBlc.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//���úڷ�ȷ���з���ťλ��
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_CONTROL_OK_BLC),
		HWND_TOP,
		rtSBlc.left+2*edge.cx,
		rtBlcTime.bottom+(int)((rtSBlc.bottom-rtBlcTime.bottom)/2),
		(int)((rtSBlc.right-rtSBlc.left-5*edge.cx)/2),
		(int)((rtSBlc.bottom-rtSBlc.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//���úڷ�ж�����水ťλ��
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_CONTROL_CANCEL_BLC),
		HWND_TOP,
		rtSBlc.right-(int)((rtSBlc.right-rtSBlc.left-edge.cx)/2),
		rtBlcTime.bottom+(int)((rtSBlc.bottom-rtBlcTime.bottom)/2),
		(int)((rtSBlc.right-rtSBlc.left-5*edge.cx)/2),
		(int)((rtSBlc.bottom-rtSBlc.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//���ð׷��������水ťλ��
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_ENGINE_LOAD_WHT),
		HWND_TOP,
		rtSWht.left+2*edge.cx,
		rtWhtTime.bottom+edge.cy,
		(int)((rtSWht.right-rtSWht.left-5*edge.cx)/2),
		(int)((rtSWht.bottom-rtSWht.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//���ð׷�ж�����水ťλ��
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_ENGINE_UNLOAD_WHT),
		HWND_TOP,
		rtSWht.right-(int)((rtSWht.right-rtSWht.left-edge.cx)/2),
		rtWhtTime.bottom+edge.cy,
		(int)((rtSWht.right-rtSWht.left-5*edge.cx)/2),
		(int)((rtSWht.bottom-rtSWht.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//���ð׷�ȷ���з���ťλ��
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_CONTROL_OK_WHT),
		HWND_TOP,
		rtSWht.left+2*edge.cx,
		rtWhtTime.bottom+(int)((rtSWht.bottom-rtWhtTime.bottom)/2),
		(int)((rtSWht.right-rtSWht.left-5*edge.cx)/2),
		(int)((rtSWht.bottom-rtSWht.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	//���ð׷�ж�����水ťλ��
	if(!SetWindowPos(
		GetDlgItem(hWnd,IDB_CONTROL_CANCEL_WHT),
		HWND_TOP,
		rtSWht.right-(int)((rtSWht.right-rtSWht.left-edge.cx)/2),
		rtWhtTime.bottom+(int)((rtSWht.bottom-rtWhtTime.bottom)/2),
		(int)((rtSWht.right-rtSWht.left-5*edge.cx)/2),
		(int)((rtSWht.bottom-rtSWht.top-7*edge.cy)/4),
		SWP_SHOWWINDOW
		))
		return false;

	return true;
}

//��䱳�������̲���
BOOL CMainWnd::FillBkAndBoard(HDC hDC)
{
	char filename[MAX_PATH]={0};	
	HFONT hFont,hOldFont;
	int x,y,len,fWidth,fHeight;
	COLORREF hOldColor;
	LOGBRUSH lb;//��ˢ������
	HBRUSH hBrush,hOldBrush;//��ˢ���

	SetBkMode(hDC,TRANSPARENT);//�������屳��ģʽΪ͸��
	
	//��䱳��ͼƬ
	if(gameSet.BkPicFileName[0]!=0)
	{
		DisplayImage(hDC,gameSet.BkPicFileName,&rtBackground);
	}
	else
	{
		lb.lbStyle=BS_SOLID;
		lb.lbColor=RGB(255,255,255);
		lb.lbHatch=NULL;
		hBrush=CreateBrushIndirect(&lb);
		hOldBrush=(HBRUSH)SelectObject(hDC,hBrush);
		FillRect(hDC,&rtBackground,hBrush);
		SelectObject(hDC,hOldBrush);
		DeleteObject(hBrush);
	}

	//������̲���
	if(gameSet.BoardFileName[0]!=0)
	{
		DisplayImage(hDC,gameSet.BoardFileName,&rtBoard);
	}

	len=strlen("Black");
	fWidth=(rtBlcPer.right-rtBlcPer.left+1)/(2*len);
	fHeight=fWidth*2;
	hFont=CreateSimpleFont(fWidth,fHeight);
	hOldFont=(HFONT)SelectObject(hDC,hFont);
	hOldColor=SetTextColor(hDC,RGB(255,0,0));
	if(gameSet.BlcPerFileName[0]!=0)
	{
		DisplayImage(hDC,gameSet.BlcPerFileName,&rtBlcPer);
		x=rtBlcPer.left+(rtBlcPer.right-rtBlcPer.left-len*fWidth)/2;
		y=rtBlcPer.bottom;			
		TextOut(hDC,x,y,"Black",len);
	}
	if(gameSet.WhtPerFileName[0]!=0)
	{
		DisplayImage(hDC,gameSet.WhtPerFileName,&rtWhtPer);
		x=rtWhtPer.left+(rtWhtPer.right-rtWhtPer.left-len*fWidth)/2;
		y=rtWhtPer.bottom;			
		TextOut(hDC,x,y,"White",len);
	}

	//�ָ�ԭʼ��ʽ
	SetTextColor(hDC,hOldColor);
	SelectObject(hDC,hOldFont);
	DeleteObject(hFont);
	return true;
}

BOOL CMainWnd::DrawCtrlBoard(HDC hDC)
{
	LOGBRUSH lb;//��ˢ������
	HBRUSH hBrush,hOldBrush;//��ˢ���
	HFONT hFont,hOldFont;//������
	int wFontName,hFontName;//�������������
	int wFontTime,hFontTime;//ʱ���������
	
	SetBkMode(hDC,TRANSPARENT);//���ñ������ģʽΪ͸��

	//��䱳��ɫΪ����ɫ
	lb.lbStyle=BS_SOLID;
	lb.lbColor=RGB(230,230,230);
	lb.lbHatch=NULL;
	hBrush=CreateBrushIndirect(&lb);
	hOldBrush=(HBRUSH)SelectObject(hDC,hBrush);
	FillRect(hDC,&rtInfoPart,hBrush);
	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);

	//**���ƺڷ�ͼ��Ԫ��
	DrawStatic(hDC,&rtSBlc,"Black(����)",RGB(230,230,230),RGB(150,150,150),edge);//���ƺڷ���̬��
	
	wFontName=(int)((rtBlcName.right-rtBlcName.left)/strlen(strBlcName));
	hFontName=(rtBlcName.bottom-rtBlcName.top);
	hFont=CreateSimpleFont(wFontName,hFontName);
	hOldFont=(HFONT)SelectObject(hDC,hFont);	
	DrawText(hDC,strBlcName,strlen(strBlcName),&rtBlcName,DT_CENTER);//���ƺڷ����������
	DeleteObject(hFont);

	wFontTime=(int)((rtBlcTime.right-rtBlcTime.left)/strlen(strBlcTime));
	hFontTime=(rtBlcTime.bottom-rtBlcTime.top);
	hFont=CreateSimpleFont(wFontTime,hFontTime);
	hOldFont=(HFONT)SelectObject(hDC,hFont);
	DrawText(hDC,strBlcTime,strlen(strBlcTime),&rtBlcTime,DT_CENTER);//���ƺڷ���������ʱ��
	DeleteObject(hFont);

	//**���ư׷�ͼ��Ԫ��
	DrawStatic(hDC,&rtSWht,"White(����)",RGB(230,230,230),RGB(150,150,150),edge);//���ư׷���̬��

	wFontName=(int)((rtWhtName.right-rtWhtName.left)/strlen(strWhtName));
	hFontName=(rtWhtName.bottom-rtWhtName.top);
	hFont=CreateSimpleFont(wFontName,hFontName);
	hOldFont=(HFONT)SelectObject(hDC,hFont);
	DrawText(hDC,strWhtName,strlen(strWhtName),&rtWhtName,DT_CENTER);//���ư׷����������
	DeleteObject(hFont);

	hFont=CreateSimpleFont(wFontTime,hFontTime);
	hOldFont=(HFONT)SelectObject(hDC,hFont);
	DrawText(hDC,strWhtTime,strlen(strWhtTime),&rtWhtTime,DT_CENTER);//���ư׷���������ʱ��
	DeleteObject(hFont);	

	return true;
}

VOID CMainWnd::AppendStepHis(char *step)
{
	strcat(StepHis, step);
	int len = strlen(StepHis);
	StepHis[len++] = 13;
	StepHis[len++] = 10;
	StepHis[len] = 0;
	SetText(hEditHis, StepHis);
}

VOID CMainWnd::SetName(char* nameCmd, int player)
{
	if (player == BLACK)
	{
		if (nameCmd == NULL)
			strcpy(strBlcName, EName);
		else
		{
			strncpy(strBlcName, nameCmd, 49);
			strBlcName[0] = 'N';
			strBlcName[4] = ':';
		}
		InvalidateRect(hWnd, &rtBlcName, FALSE);
	}
	else if (player == WHITE)
	{
		if (nameCmd == NULL)
			strcpy(strBlcName, EName);
		else
		{
			strncpy(strWhtName, nameCmd, 49);
			strWhtName[0] = 'N';
			strWhtName[4] = ':';
		}
		InvalidateRect(hWnd, &rtWhtName, FALSE);
	}
}

VOID CMainWnd::ShowWiner(int side)
{
	if (side == BLACK)
	{
		MessageBox(hWnd, "�ڷ�ʤ����", "Msg", MB_OK);
	}
	else
	{
		MessageBox(hWnd, "�׷�ʤ����", "Msg", MB_OK);
	}
}

VOID CMainWnd::GameStart()
{
	strcpy(StepHis, "Step History\r\n"
		"------------------------------------\r\n");
	strcpy(strBlcTime, "Time: 00:00");
	strcpy(strWhtTime, "Time: 00:00");
	InvalidateRect(hWnd, &rtBlcTime, FALSE);
	InvalidateRect(hWnd, &rtWhtTime, FALSE);
	timer.StartTimer(hWnd);
}

VOID CMainWnd::GameStop()
{
	timer.StopTimer();
}

VOID CMainWnd::UpdateTime()
{
	if (timer.UpdateTime(strBlcTime, strWhtTime) == 0)
		InvalidateRect(hWnd, &rtBlcTime, FALSE);
	else
		InvalidateRect(hWnd, &rtWhtTime, FALSE);
}