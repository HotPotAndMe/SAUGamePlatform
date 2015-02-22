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
	//��ʼ�����
	virtual VOID InitGame();
	//����������������  ����ֵ��  -1������Υ��  0��δ�ҵ��ؼ���  1����ȡ�ɹ�  2��ʤ����
	virtual INT ProcessMove(char *moveCmd, char *curCmd, char *denCmd);
	//��Ӧ��굥����Ϣ
	virtual BOOL OnLButtonDown(int x, int y);
	//ȷ���ŷ�
	virtual INT OkMove(char *denCmd);
	//ȡ���ŷ�
	virtual INT CancelMove();

private:

	BYTE board[10][10];//10*10����
	stack<Step> stepStack;//�ŷ�ջ

	HPEN hPen;//���̿��߻��ʾ��
	HFONT hFont;//�̶�������
	HBRUSH hBrush;//���̻�ˢ
	HDC hBlcDC, hWhtDC, hBarDC, hMarkDC;//����Ԫ��DC

	int d;//���̿��߼��
	int pWidth;//���߻��ʿ�
	int fWidth, fHeight;//�̶��������

	//��������
	VOID DrawChess();
	//����������Ч
	bool PlaySnd(int tag);
	//��ʼ������״̬
	VOID InitBoard();
	//˫������
	BOOL SToS(Point point);
	//�ж�ʤ��
	bool WinOrLose();
	//�ж��岽�Ϸ���
	bool FitRules();
	//�ж�����
	bool JudgeRule(Point src, Point des);

};

#endif