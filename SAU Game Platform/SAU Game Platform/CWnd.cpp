//FileName: CWnd.cpp
//Version: 1.1
//Date: 2013/10/24
//Description: SAU_GamePalform ���򴰿ڻ����ļ������ݰ�����CWnd�������ĳ�Ա�����Ķ��塣

#include "CWnd.h"

//���캯��
CWnd::CWnd()
{
	ClsName="WINDOW";
	WndName="Window";
	hWnd=NULL;
}

//��������
CWnd::~CWnd()
{

}

VOID CWnd::SetWindowStyle(int nIndex,LONG dwNewLong)
{
	SetWindowLong(hWnd,nIndex,dwNewLong);
}

//�жϵ��ھ�����
bool CWnd::InsideRect(const RECT* rt,const int &x,const int &y)
{
	if((rt->left+ALLOW)<x && x<(rt->right-ALLOW) && (rt->top+ALLOW)<y && y<(rt->bottom-ALLOW))
		return true;
	else
		return false;
}

//��������
HFONT CWnd::CreateSimpleFont(int width,int height)
{
	HFONT hFont;
	hFont=CreateFont(
		height,//�߶�
		width,//���
		0,
		0,
		FW_NORMAL,//����Ȩֵ����ϸ��
		FALSE,//б��
		FALSE,//�»���
		FALSE,//ɾ����
		DEFAULT_CHARSET,//�ַ���
		OUT_DEFAULT_PRECIS,//�������
		CLIP_DEFAULT_PRECIS,//�ü�����
		DEFAULT_QUALITY,//�������
		DEFAULT_PITCH,//�������������
		NULL//������
		);
	return hFont;
}

//���ƾ�̬��
VOID CWnd::DrawStatic(HDC hDC,RECT *rt,LPCTSTR text,COLORREF BkColor,COLORREF PenColor,SIZE edge)
{
	HPEN hPen,hOldPen;
	HFONT hFont,hOldFont;
	RECT rect;
	int backgroundMode=GetBkMode(hDC);
	COLORREF textColor=GetTextColor(hDC);

	SetBkMode(hDC,OPAQUE);
	SetBkColor(hDC,BkColor);

	hPen=CreatePen(PS_SOLID,1,PenColor);
	hOldPen=(HPEN)SelectObject(hDC,hPen);	
	MoveToEx(hDC,rt->left+edge.cx,rt->top+edge.cy,NULL);
	LineTo(hDC,rt->right-edge.cx,rt->top+edge.cy);
	LineTo(hDC,rt->right-edge.cx,rt->bottom-edge.cy);
	LineTo(hDC,rt->left+edge.cx,rt->bottom-edge.cy);
	LineTo(hDC,rt->left+edge.cx,rt->top+edge.cy);

	hFont=CreateSimpleFont(edge.cx,2*edge.cy);
	hOldFont=(HFONT)SelectObject(hDC,hFont);
	rect.left=(int)(rt->left+(rt->right-rt->left-strlen(text)*edge.cx)/2);
	rect.right=rect.left+strlen(text)*edge.cx;
	rect.top=rt->top;rect.bottom=rt->top+edge.cy*2;
	DrawText(hDC,text,strlen(text),&rect,DT_CENTER);

	SetBkColor(hDC,textColor);
	SetBkMode(hDC,backgroundMode);
	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);
	SelectObject(hDC,hOldFont);
	DeleteObject(hFont);
	return;
}
