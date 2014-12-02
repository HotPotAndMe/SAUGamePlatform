#ifndef CCONSIX_H_GAMEPLATFORM
#define CCONSIX_H_GAMEPLATFORM

#include "..\include\CChess.h"


#define EMPTY 2//��


struct Step{
	Point first, second;
	BOOL side;
};
#define InitStep(step) { step.first.x=step.first.y=step.second.x=step.second.y=-1; }
#define IsChess(point) if(point.x!=-1&&point.y!=-1)

class CConSix : public CChess
{
public:

	CConSix();
	~CConSix();

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

	BYTE board[19][19];//������Ϣ�ṹ

	bool first_hand;//��һ�ֱ�־(�����������һ����һ�ӣ��Ժ�ÿ�������ӣ�����Ҫ���)

	COLORREF BkColor;//����ɫ
	COLORREF BoardColor;//������ɫ
	HPEN hPen;//���ʾ��
	HFONT hFont;//������

	int d;//���������̿��߼��
	double pixel;//���������̿��߿��
	int fWidth, fHeight;//�������

	stack<Step> stepStack;//�ŷ�ջ

	//��������
	bool DrawChess(HDC hBlcDC, HDC hCurBlcDC, HDC hWhtDC, HDC hCurWhtDC, int d);
	//��ʼ������״̬
	VOID InitBoard();
	//˫������
	BOOL SToS(int x, int y);
	//�ж�ʤ��
	bool WinOrLose();
	//�ж��岽�Ϸ���
	bool FitRules();
	//��ʾ�з���ʷ
	VOID ShowStepHis(char *msg);
	//����������Ч
	bool PlaySnd(int sel);

};

#endif