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

	CConSix(HINSTANCE hInst,HWND hWnd,char *LibPath);
	~CConSix();

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

	BYTE board[19][19];//������Ϣ�ṹ
	stack<Step> stepStack;//�ŷ�ջ

	bool first_hand;//��һ�ֱ�־(�����������һ����һ�ӣ��Ժ�ÿ�������ӣ�����Ҫ���)

	HPEN hPen;//���̿��߻��ʾ��
	HFONT hFont;//�̶�������
	HDC hBlcDC, hWhtDC, hMarkDC;//����Ԫ��DC

	int d;//���̿��߼��
	int pWidth;//���߻��ʿ�
	int fWidth, fHeight;//�̶��������

	//��������
	VOID DrawChess();
	//������Ч
	bool PlaySnd(int tag);
	//��ʼ������״̬
	VOID InitBoard();
	//˫������
	BOOL SToS(Point point);
	//�ж��岽�Ϸ���
	bool FitRules();
	//�ж�ʤ��
	bool WinOrLose();

};

#endif