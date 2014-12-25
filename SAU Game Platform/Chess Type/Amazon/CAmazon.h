#ifndef CAMAZON_H_GAMEPLATFORM
#define CAMAZON_H_GAMEPLATFORM

#include "..\include\CChess.h"


#define EMPTY 2//��
#define	BAR 3//�ϰ�

struct Step
{
	Point first, second, third;
	BYTE side;
};
#define InitStep(step) { step.first.x=step.first.y=step.second.x=step.second.y=step.third.x=step.third.y=-1; }
#define IsChess(point) if(point.x!=-1&&point.y!=-1)


class CAmazon : public CChess
{
public:	

	CAmazon(HINSTANCE hInst, HWND hWnd, char *LibPath);
	~CAmazon();

	//�������̴�С
	virtual VOID SetBoard(RECT rtBoard);
	//��������
	virtual VOID DrawBoard(HDC hDC);
	//����������������  ����ֵ��  -1������Υ��  0��δ�ҵ��ؼ���  1����ȡ�ɹ�  2��ʤ����
	virtual INT ProcessMove(char *moveCmd);
	//��ʼ�����
	virtual VOID InitGame();
	//��Ӧ��굥����Ϣ
	virtual BOOL OnLButtonDown(int x, int y);
	//ȷ���ŷ�
	virtual INT OkMove();
	//ȡ���ŷ�
	virtual VOID CancelMove();

private:

	BYTE board[10][10];//10*10����

	COLORREF BkColor;//����ɫ
	COLORREF BoardColor;//������ɫ
	HPEN hPen;//���ʾ��
	HFONT hFont;//������
	HBRUSH hBrush;//���̻�ˢ
	HDC hBlcDC, hWhtDC, hBarDC, hMarkDC;

	int d;//����ѷ�����̿��߼��
	double pixel;//����ѷ�����̿��߿��
	int fWidth,fHeight;//�������

	stack<Step> stepStack;//�ŷ�ջ

	//��������
	bool DrawChess();
	//��ʼ������״̬
	VOID InitBoard();
	//˫������
	BOOL SToS(Point point);
	//�ж�ʤ��
	bool WinOrLose();
	//�ж��岽�Ϸ���
	bool JudgeRule(Point src, Point des);
	bool FitRules();
	//��ʾ�з���ʷ
	VOID ShowStepHis(char *msg);
	//����������Ч
	bool PlaySnd(int sel);
};

#endif