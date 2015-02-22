#ifndef CSURAKARTA_H_GAMEPLATFORM
#define CSURAKARTA_H_GAMEPLATFORM

#include <vector>
#include "../include/CChess.h"

using namespace std;


#define EMPTY 2

struct PointF
{
	double x;
	double y;
};

struct PointS
{
	int x, y;
	int side;
};

struct Step{
	Point start,end;
	bool flagE;//���ӱ��
	BOOL side;
};
#define InitStep(step) { step.start.x=step.start.y=step.end.x=step.end.y=-1;step.flagE=false; }
#define IsChess(point) if(point.x!=-1&&point.y!=-1)


class CSurakarta : public CChess
{
public:	

	CSurakarta(HINSTANCE hInst, HWND hWnd, char *LibPath);
	~CSurakarta();

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

	BYTE board[6][6];//����
	stack<Step> stepStack;//�ŷ�ջ
	vector<Point> MoveWay;//����·��
	PointS ptMovie;//������������

	HPEN hRPen,hGPen,hBPen;//���߻��ʾ��
	HFONT hFont;//�̶�������
	HDC hMarkDC, hBlcDC, hWhtDC;//����Ԫ��DC

	int d;//���̿��߼��
	int pWidth;//���߿��
	int fWidth,fHeight;//�̶��������
	int size;//����ֱ��


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

	//����
	VOID MoveChess();
	//���ų��Ӷ���
	VOID PlayMovie();
	//��������·��
	bool SearchWay(Step sp);
	//�������·��
	VOID SaveWay(Point *way, int start, int end, bool dir);
	//���ɶ���·��
	VOID SavePtWay(vector<Point> *ptWay);

};

#endif