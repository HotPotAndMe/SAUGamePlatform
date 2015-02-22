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

	BYTE line[2][6][5];//2��6��5��  [0����/1����]
	BYTE box[5][5];//6*6�ĵ㣬5*5�ĸ���
	stack<Step> stepStack;//�ŷ�ջ
	char lineList[256];//�����б�
	int lineNum;//�������߲������������������ߵ�ģʽ

	HFONT hFont;//�̶�������
	HDC hBlcDC, hWhtDC, hPtDC;//����Ԫ��DC

	int d;//���߼��
	int pWidth;//�ߵĿ�
	int fWidth,fHeight;//�������
	int pR, pL;//��İ뾶��ֱ��
	int size;//���

	int BE;

	//��������
	bool DrawChess();
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
	
	//���������ж�
	bool HaveLine(Point point);
	//��������ж�
	bool HaveBox(LINE line);
	//����ӳ��
	bool PonitToLine(Step &step);
	bool LineToPoint(Step &step);

	VOID GetConfig();

};

#endif