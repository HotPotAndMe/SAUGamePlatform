#ifndef CDOTSANDBOXES_H_GAMEPLATFORM
#define CDOTSANDBOXES_H_GAMEPLATFORM

#include "..\include\CChess.h"


#define EMPTY 2

typedef struct
{
	int k;//2��0����1����
	int i;//6��
	int j;//5��
}LINE;

struct Step
{
	Point start, end;//������ֹλ��
	LINE line;//�ߵ�����
	BYTE side;
};
#define InitStep(step) { step.start.x=step.start.y=step.end.x=step.end.y=step.line.k=step.line.i=step.line.j=-1; }
#define IsChess(point) if(point.x!=-1&&point.y!=-1)
#define IsLine(line) if(line.k!=-1&&line.i!=-1&&line.j!=-1)

class CDotsAndBoxes : public CChess
{
public:

	CDotsAndBoxes(HINSTANCE hInst, HWND hWnd, char *LibPath);
	~CDotsAndBoxes();

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

	BYTE line[2][6][5];//2��6��5��  [0����/1����]
	BYTE box[5][5];//6*6�ĵ㣬5*5�ĸ���
	int connectS;//�������߲������������������ߵ�ģʽ

	COLORREF BkColor;//����ɫ
	COLORREF BoardColor;//������ɫ
	HPEN hPen;//���ʾ��
	HFONT hFont;//������

	HDC hBlcDC, hWhtDC, hPtDC;

	int d;//��������̿��߼��
	double pixel;//��������̿��߿��
	int fWidth,fHeight;//�������
	int pR, pWidth, pHeight;

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

	//���������ж�
	bool HaveLine(Point point);
	//��������ж�
	bool HaveBox(LINE line);
	//����ӳ��
	bool PonitToLine(Step &step);
	bool LineToPoint(Step &step);

};

#endif