#include "CChess.h"


/**
 * ErrorBox - ������ʾ��
 * @ErrorInfo:	������Ϣ
 */
VOID __cdecl ErrorBox(LPTSTR ErrorInfo)
{
	CHAR error1[50], error2[20];
	strcpy(error1, ErrorInfo);
	sprintf(error2, "\n\nerror: %d", GetLastError());
	strcat(error1, error2);
	MessageBox(NULL, error1, "error", MB_OK);
}

CChess::CChess()
{	
	count = -1;
	player=BLACK;
	memset(StepNum, 0, sizeof(StepNum));
	memset(curCmd, 0, sizeof(curCmd));//д���������Ϣ
	memset(denCmd, 0, sizeof(denCmd));
}

CChess::~CChess()
{
}

VOID CChess::GetConfig()
{
	char filename[MAX_PATH];
	sprintf(filename, "%s\\Config.ini", LibPath);
	BX = GetPrivateProfileInt("COORDINATE", "X", 'A', filename);
	BY = GetPrivateProfileInt("COORDINATE", "Y", 'A', filename);
}

VOID CChess::ShowStepHis(char *msg)
{
	char step[50];
	memset(step, 0, sizeof(step));
	if (player == BLACK)
	{
		strcpy(step, "Black: ");
	}
	else if (player == WHITE)
	{
		strcpy(step, "White: ");
	}
	strcat(step, msg);
	SendMessage(hWnd, GM_SHOWSTEP, (WPARAM)step, (LPARAM)player);
	return;
}

//���ض���С�ĵ�(Բ��)
bool CChess::DrawPoint(HDC hDC,int r,COLORREF ptColor,COLORREF bkColor)
{	
	for(int i=1;i<=2*r;i++)
	{
		for(int j=1;j<=2*r;j++)
		{
			if((i-r)*(i-r)+(j-r)*(j-r)<=r*r)
				SetPixel(hDC,i-1,j-1,ptColor);
			else
				SetPixel(hDC,i-1,j-1,bkColor);
		}		
	}
	return true;
}

//����Բ�θ���λͼ(��͸������)
bool CChess::DrawAssist(HDC hAssistDC,int r)
{
	for(int i=1;i<=r*2;i++)
	{
		for(int j=1;j<=r*2;j++)
		{
			if ((i - r)*(i - r) + (j - r)*(j - r)<=r*r)//Բ��
				SetPixel(hAssistDC,i-1,j-1,RGB(255,255,255));//��ɫ
			else//Բ��
				SetPixel(hAssistDC,i-1,j-1,RGB(0,0,0));//��ɫ
		}
	}
	return true;
}

HFONT CChess::CreateSimpleFont(int width,int height)//����һ������
{
	HFONT hFont;
	hFont=CreateFont(
		height,
		width,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		NULL
		);
	return hFont;
}

bool CChess::InsideRect(const RECT* rt,const int &x,const int &y)//�жϵ��Ƿ��ھ�����
{
	if((rt->left+ALLOW)<x && x<(rt->right-ALLOW) && (rt->top+ALLOW)<y && y<(rt->bottom-ALLOW))
		return true;
	else
		return false;
}
