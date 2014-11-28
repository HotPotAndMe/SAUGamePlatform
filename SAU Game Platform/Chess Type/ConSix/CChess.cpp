#include "CChess.h"


CChess::CChess()
{	
	player=BLACK;
	memset(StepNum, 0, sizeof(StepNum));
	memset(curCmd, 0, sizeof(curCmd));//д���������Ϣ
	memset(denCmd, 0, sizeof(denCmd));
}

CChess::~CChess()
{

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

//���ض���С�ĵ�(Բ��)
bool CChess::DrawPoint(HDC hDC,int r,COLORREF ptColor,COLORREF bkColor)
{	
	for(int i=0;i<=2*r;i++)
	{
		for(int j=0;j<=2*r;j++)
		{
			if((i-r)*(i-r)+(j-r)*(j-r)<r*r)
			{
				SetPixel(hDC,i,j,ptColor);
			}
			else
			{
				SetPixel(hDC,i,j,bkColor);
			}
		}		
	}
	return true;
}

bool CChess::InsideRect(const RECT* rt,const int &x,const int &y)//�жϵ��Ƿ��ھ�����
{
	if((rt->left+ALLOW)<x && x<(rt->right-ALLOW) && (rt->top+ALLOW)<y && y<(rt->bottom-ALLOW))
		return true;
	else
		return false;
}

//���Ƹ���λͼ(��͸������)
bool CChess::DrawAssist(HDC hAssistDC,int d)
{
	for(int i=0;i<d;i++)
	{
		for(int j=0;j<d;j++)
		{
			if(pow((double)(i-d/2),2)+pow((double)(j-d/2),2)<=pow((double)d/2,2))
			{
				SetPixel(hAssistDC,i,j,RGB(255,255,255));
			}
			else
			{
				SetPixel(hAssistDC,i,j,RGB(0,0,0));
			}
		}
	}
	return true;
}

//�����ı����ı�����
VOID SetText(HWND hWnd,LPCTSTR str)
{
	SendMessage(hWnd,WM_SETTEXT,(WPARAM)0,(LPARAM)str);	//�������
	SendMessage(hWnd,EM_SETSEL,(WPARAM)-2,(LPARAM)-1);	//���ý��㵽ĩβ
	SendMessage(hWnd,WM_VSCROLL,SB_BOTTOM,0);
}

VOID GetText(HWND hWnd,LPCTSTR str,int size)//��ȡ�ı����ı�����
{	
	SendMessage(hWnd,WM_GETTEXT,size,(LPARAM)str);
}