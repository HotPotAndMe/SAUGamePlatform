#ifndef CNOGO_H_GAMEPLATFORM
#define CNOGO_H_GAMEPLATFORM

#include "..\include\CChess.h"

#define EMPTY 2
#define MARK 1


struct Step{
	Point point;
	BOOL side;
};
#define InitStep(step) { step.point.x=step.point.y=-1; }
#define IsChess(point) if(point.x!=-1&&point.y!=-1)


class CNoGo : public CChess
{
public:	

	CNoGo(HINSTANCE hInst, HWND hWnd, char *LibPath);
	~CNoGo();

	//�������̴�С
	virtual VOID SetBoard(RECT rtBoard);
	//��������
	virtual VOID DrawBoard(HDC hDC);
	//��ʼ�����
	virtual VOID InitGame();
	//����������������  ����ֵ��  -1������Υ��  0��δ�ҵ��ؼ���  1����ȡ�ɹ�  2��ʤ����
	virtual INT ProcessMove(char *moveCmd);
	//��Ӧ��굥����Ϣ
	virtual BOOL OnLButtonDown(int x, int y);
	//ȷ���ŷ�
	virtual INT OkMove();
	//ȡ���ŷ�
	virtual INT CancelMove();

private:

	BYTE board[9][9];//����
	stack<Step> stepStack;//�ŷ�ջ
	int Biaoji[81];//��ǣ���������

	HPEN hPen;//���̿��߻��ʾ��
	HFONT hFont;//�̶�������
	HDC hBlcDC, hWhtDC, hMarkDC;//����Ԫ��DC

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
	//�ж��岽�Ϸ���
	bool FitRules();
	//�ж�ʤ��
	bool WinOrLose();

	//��ȡ���ӵ�����
	int GetQi(int x, int y, int side);
	int GetQiForPoint(int x, int y, int side);

};

#endif