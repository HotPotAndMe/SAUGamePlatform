/**
* file: 
* date: 
* version: 
* description: 
*/

#ifndef CMAINWND_H_GAMEPLATFORM
#define CMAINWND_H_GAMEPLATFORM

#include "CWnd.h"
#include "CTime.h"


#define MAINWNDTIMER 1
#define RATIO 0.8//������ʾ����ռ���������
#define MARGIN 10
#define PI 3.14159 //�߽硢�����������ʼ����


class CMainWnd :public CWnd//��������
{
public:
	HINSTANCE hRE;//��ǰ���õ���Դʵ�����
	HMENU hCurMenu;//��ǰ�˵����

	//���캯��
	CMainWnd();
	//��������
	~CMainWnd();

	friend void PSDrawBoard(HDC hDC);
		
	VOID SetName(char* nameCmd, int player);
	VOID GameStart();
	VOID GameStop();
	VOID UpdateTime();

	BOOL RegisterWnd(HINSTANCE hInst);
	BOOL CreateWnd(HWND hParentWnd=NULL,HMENU hMenu=NULL);
	LRESULT WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	RECT GetBoardPos(){return this->rtBoard;}

private:
	CTime timer;//��ʱ��

	RECT rtBoardPart;//������ʾ����
	RECT rtInfoPart;//��Ϣ����ʾ����

	RECT rtBackground;//����λ��
	int BoardSide;//���̱߳�
	RECT rtBoard;//����λ��
	RECT rtBlcPer;//�ڷ�����ͼƬλ��
	RECT rtWhtPer;//�׷�����ͼƬλ��

	SIZE edge;//��׼��
	RECT rtSBlc;//�ڷ������
	RECT rtSWht;//�׷������
	RECT rtBlcName;//�ڷ����־�̬��
	RECT rtWhtName;//�׷����־�̬��
	RECT rtBlcTime;//�ڷ�ʱ�侲̬��
	RECT rtWhtTime;//�׷�ʱ�侲̬��

	char strBlcName[50];//�ڷ�������
	char strWhtName[50];//�׷�������

	char strBlcTime[20];//�ڷ���ʱ
	char strWhtTime[20];//�׷���ʱ

	HWND hEditHis;//��ʷ�з���Ϣ�ı���

	char StepHis[5000];	//�з���ʷ��˫������ʤ���������Ϊ100����һ����Ϣ���Ϊ50�ֽڼ��㡣

	VOID AppendStepHis(char *step);
	VOID ShowWiner(int side);

	//��ȡ�����ڿͻ���RECT(������״̬��)
	VOID GetClientRectEx(RECT *rt);
	
	BOOL AdjustWndPos();
	//�����������ͼ��Ԫ��λ��
	VOID GetBkAndBoardRT();
	//�����ؼ�
	BOOL CreateCtrl(HWND hWnd);
	//�����ؼ�λ��
	BOOL AdjustCtrlPos();

	BOOL DrawBoard(HDC hMenDC);
	BOOL FillBkAndBoard(HDC hMemDC);
	BOOL DrawCtrlBoard(HDC hMemDC);

	VOID OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam);
	VOID OnPaint(WPARAM wParam,LPARAM lParam);
	BOOL OnEraseBkgnd(WPARAM wParam,LPARAM lParam);	
	VOID OnSize(WPARAM wParam,LPARAM lParam);
	VOID OnNotify(WPARAM wParam,LPARAM lParam);
	VOID OnCommand(WPARAM wParam,LPARAM lParam);
	VOID OnMenuSelect(WPARAM wParam,LPARAM lParam);
	VOID OnLButtonDown(WPARAM wParam,LPARAM lParam);
	VOID OnLButtonDbClick(WPARAM wParam,LPARAM lParam);
	VOID OnRButtonDown(WPARAM wParam,LPARAM lParam);
	VOID OnMouseMove(WPARAM wParam,LPARAM lParam);
	VOID OnKeyDown(WPARAM wParam,LPARAM lParam);
	VOID OnTimer(WPARAM wParam,LPARAM lParam);
	VOID OnClose(WPARAM wParam,LPARAM lParam);
	VOID OnDestroy(WPARAM wParam,LPARAM lParam);
	VOID OnName(WPARAM wParam,LPARAM lParam);
};

extern CMainWnd *MainWnd;//�����ڶ���ָ��


#endif
