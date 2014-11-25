//FileName: CWnd.h
//Version: 1.1
//Date: 2013/10/24
//Description: SAU_GamePalform ���򴰿ڻ���ͷ�ļ������ݰ�����CWnd��Ķ��弰���Ա������������

#ifndef CWND_H_GAMEPLATFORM
#define CWND_H_GAMEPLATFORM


#define ALLOW 5


#include "Game Platform.h"

class CWnd//������
{
public:
	HWND hWnd;//���ھ��
	
	RECT rtWindow;//����RECT
	RECT rtClient;//�ͻ���RECT

	//���캯��
	CWnd();
	//��������
	~CWnd();		

	//ע�ᴰ����
	virtual BOOL RegisterWnd(HINSTANCE hInst)=0;
	//��������
	virtual BOOL CreateWnd(HWND hParentWnd=NULL,HMENU hMenu=NULL)=0;
	virtual LRESULT WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)=0;
	

	//���ô�������
	VOID SetWindowStyle(int nIndex,LONG dwNewLong);
	//�жϵ��ھ�����
	static bool InsideRect(const RECT* rt,const int &x,const int &y);
	//��������
	static HFONT CreateSimpleFont(int width,int height);
	//���ƾ�̬��
	static VOID DrawStatic(HDC hDC,RECT *rt,LPCTSTR text,COLORREF BkColor,COLORREF PenColor,SIZE edge);

protected:
	LPTSTR WndName;//���ڱ���
	LPTSTR ClsName;	//��������


	//��Ӧ��Ϣ

	//��Ӧ���崴��
	virtual VOID OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam)=0;
	//��Ӧ��ͼ
	virtual VOID OnPaint(WPARAM wParam,LPARAM lParam)=0;
	//��Ӧ��������
	virtual BOOL OnEraseBkgnd(WPARAM wParam,LPARAM lParam)=0;	
	//��Ӧ���ڴ�С�ı�
	virtual VOID OnSize(WPARAM wParam,LPARAM lParam)=0;
	virtual VOID OnNotify(WPARAM wParam,LPARAM lParam)=0;
	//��Ӧ�ؼ���Ϣ
	virtual VOID OnCommand(WPARAM wParam,LPARAM lParam)=0;
	//��Ӧ�˵���ѡ���޸�״̬����ʾ��
	virtual VOID OnMenuSelect(WPARAM wParam,LPARAM lParam)=0;
	virtual VOID OnLButtonDown(WPARAM wParam,LPARAM lParam)=0;
	virtual VOID OnLButtonDbClick(WPARAM wParam,LPARAM lParam)=0;
	virtual VOID OnRButtonDown(WPARAM wParam,LPARAM lParam)=0;
	virtual VOID OnMouseMove(WPARAM wParam,LPARAM lParam)=0;
	//���̰�����Ϣ
	virtual VOID OnKeyDown(WPARAM wParam,LPARAM lParam)=0;
	virtual VOID OnTimer(WPARAM wParam,LPARAM lParam)=0;
	//��Ӧ���ڹر���Ϣ
	virtual VOID OnClose(WPARAM wParam,LPARAM lParam)=0;
	//��Ӧ����������Ϣ
	virtual VOID OnDestroy(WPARAM wParam,LPARAM lParam)=0;
};

#endif