#ifndef CCONSIX_H_GAMEPLATFORM
#define CCONSIX_H_GAMEPLATFORM

#include "CChess.h"

class CConSix : public CChess
{
public:	
	HPEN hPen;//���ʾ��
	HFONT hFont;//������

	int d;//���������̿��߼��
	double pixel;//���������̿��߿��
	int fWidth,fHeight;//�������

	BYTE board[19][19];//������Ϣ�ṹ
	step curStep;//��ǰ��

	bool first_hand;//��һ�ֱ�־(�����������һ����һ�ӣ��Ժ�ÿ�������ӣ�����Ҫ���)
	int count;

	CConSix();
	~CConSix();

	//��������
	bool DrawBoard(HDC hDC);
	//��������
	bool DrawChess(HDC hBlcDC,HDC hCurBlcDC,HDC hWhtDC,HDC hCurWhtDC,int d);
	//����������Ϣ  ����ֵ��  -1������Υ��  0��δ�ҵ��ؼ���  1����ȡ�ɹ�  2��ʤ����
	BOOL ProcessMsg(char *msg);
	//�ж�ʤ��
	bool WinOrLose(BYTE side);
	//�ж��岽�Ϸ���
	bool FitRules(step sp);
	//��ʼ�����
	VOID InitGame();
	//��ʼ������״̬
	VOID InitBoard();
	//��Ӧ��굥����Ϣ
	BOOL OnLButtonDown(int x,int y);	
	//��ʾ�з���ʷ
	VOID ShowStepHis(char *msg);
	//˫������
	BOOL SToS(int x,int y);
	//����������Ч
	bool PlaySnd(int sel);
};

#endif