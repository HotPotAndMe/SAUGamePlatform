#ifndef CPHANYOMGO_H_GAMEPLATFORM
#define CPHANTOMGO_H_GAMEPLATFORM

#include "..\include\CChess.h"

#define EMPTY 2
#define MARK 1


struct Step{
	Point point;
	BOOL side;
};
#define InitStep(step) { step.point.x=step.point.y=-1; }
#define IsChess(point) if(point.x!=-1&&point.y!=-1)


class CPhantomGo : public CChess
{
public:	

	CPhantomGo(HINSTANCE hInst, HWND hWnd, char *LibPath);
	~CPhantomGo();

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

	BYTE board[9][9];//����
	int Biaoji[81],NoQi[81];
	Point Jie;//�ٵ�
	bool flagJ;//�ٱ��
	char takeList[256];
	int takeNum;
	bool flagP;//pass��ǣ������վ�ȷ����

	COLORREF BkColor;//����ɫ
	COLORREF BoardColor;//������ɫ
	HPEN hPen;//���ʾ��
	HFONT hFont;//������
	HDC hBlcDC, hWhtDC, hMarkDC;

	int d;//��Χ�����̿��߼��
	double pixel;//��Χ�����̿��߿��
	int fWidth, fHeight;//�������

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
	bool FitRules();
	//����������Ч
	bool PlaySnd(int sel);

	int GetQiForPoint(int x, int y, int side);
	int GetQi(int x, int y, int side);
	bool Take(int x, int y, int side);

};

#endif