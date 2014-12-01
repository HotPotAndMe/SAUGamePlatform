#ifndef CCHESS_H_GAMEPLATFORM
#define CCHESS_H_GAMEPLATFORM

#include <Windows.h>
#include <math.h>


#define ALLOW 5
#define BUFSIZE (1024*512)//�ܵ���С512K

#define GM_SHOWSTEP		0X1012
#define GM_WINLOSE		0X1013

#define BLACK 0//�ڷ�
#define WHITE 1//�׷�
#define NEXTPLAYER(S) (1-S)//�ֻ�����Ȩ


typedef struct
{
	char* ChessName;//����
	char* BlcName;//�ڷ���������
	char* WhtName;//�׷���������
	char* res;//�������
	char* BlcTime;//�ڷ�������ʱ
	char* WhtTime;//�׷�������ʱ
	char* BlcStep;//�ڷ�����
	char* WhtStep;//�׷�����
	char* BlcTimePerStep;//�ڷ�ƽ��ÿ��ʱ��
	char* WhtTimePerStep;//�׷�ƽ��ÿ��ʱ��
	char* GameDate;//��������ʱ��
}GAMEINFO;


class CChess
{
public:	
	HWND hWnd;//���ھ��

	char LibPath[MAX_PATH];//����֧��ģ��·��

	char curCmd[256];//��д�뱾����������
	char denCmd[256];//��д��Է���������

	BYTE player;//���巽

	CChess();
	~CChess();

	//�������̴�С
	virtual VOID SetBoard(RECT rtBoard) = 0;
	//��������
	virtual VOID DrawBoard(HDC hDC) = 0;
	//��ʼ�����
	virtual VOID InitGame() = 0;
	//����������������  ����ֵ��  -1������Υ��  0��δ�ҵ��ؼ���  1����ȡ�ɹ�  2��ʤ����
	virtual INT ProcessMove(char *msg) = 0;
	//��Ӧ��굥����Ϣ
	virtual BOOL OnLButtonDown(int x, int y)=0;
	//ȷ���ŷ�
	virtual INT OkMove() = 0;
	//ȡ���ŷ�
	virtual VOID CancelMove() = 0;

	//���Ƶ㣨Բ�Σ�
	static bool DrawPoint(HDC hDC,int r,COLORREF ptColor,COLORREF bkColor);	
	//����Բ�θ���λͼ
	static bool DrawAssist(HDC hAssistDC,int r);
	//����������
	static HFONT CreateSimpleFont(int width,int height);
	//�жϵ��Ƿ���������
	static bool InsideRect(const RECT* rt,const int &x,const int &y);

protected:
	RECT rtBoard;//����λ��
	int side;//���̿��
	int StepNum[2];//�ڰ�˫���Ĳ���
	int count;//���������״̬��-1Ϊ���������������
};

#endif
