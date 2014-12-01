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

//���ض���С�ĵ�(Բ��)
bool CChess::DrawPoint(HDC hDC,int r,COLORREF ptColor,COLORREF bkColor)
{	
	for(int i=0;i<=2*r;i++)
	{
		for(int j=0;j<=2*r;j++)
		{
			if((i-r)*(i-r)+(j-r)*(j-r)<r*r)
				SetPixel(hDC,i,j,ptColor);
			else
				SetPixel(hDC,i,j,bkColor);
		}		
	}
	return true;
}

//����Բ�θ���λͼ(��͸������)
bool CChess::DrawAssist(HDC hAssistDC,int r)
{
	for(int i=0;i<r*2;i++)
	{
		for(int j=0;j<r*2;j++)
		{
			if ((i - r)*(i - r) + (j - r)*(j - r)<r*r)//Բ��
				SetPixel(hAssistDC,i,j,RGB(255,255,255));//��ɫ
			else//Բ��
				SetPixel(hAssistDC,i,j,RGB(0,0,0));//��ɫ
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
