#ifndef CCHESS_H_GAMEPLATFORM
#define CCHESS_H_GAMEPLATFORM

#include <Windows.h>
#include <math.h>

#define BLACK 0//��
#define WHITE 1//��
#define EMPTY 2//��
#define	BAR 3//�ϰ�
#define UPLEFT 0//���Ͻ�
#define DOWNLEFT 1//���½�
#define CENTER 2//����
#define ALLOW 5
#define BUFSIZE (1024*512)//�ܵ���С512K

#define GM_SHOWSTEP		0X1012
#define GM_WINLOSE		0X1013

#define NEXT(S) (1-S)

typedef struct
{
	int x;
	int y;
	BYTE side;
}point;

typedef struct
{
	POINT first;
	POINT second;
	BYTE side;
}step;

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

//��������
typedef struct _GameSet{
	//·������
	char CurDir[MAX_PATH-100];//��ǰӦ�ó����Ŀ¼
	char BoardFileName[MAX_PATH];//���̲���·��
	char BkPicFileName[MAX_PATH];//����ͼƬ·��
	char BlcPerFileName[MAX_PATH];//�ڷ�����ͼƬ·��
	char WhtPerFileName[MAX_PATH];//�׷�����ͼƬ·��
	char BkMusicFileName[MAX_PATH];//��������·��
	char cmDir[MAX_PATH-100];//���ױ���·��
	char PrintScrDir[MAX_PATH-100];//��ͼ·��
	char cmFileName[MAX_PATH];//�����ļ�·��
	char EngineInitDir[MAX_PATH-100];//�����ʼĿ¼

	//ƽ̨�������־
	bool fullScr;//ȫ�� ��־
	bool SingleStep;//������������ ��־
	bool AllStep;//һ�𱣴����� ��־
	bool PrintScr;//��ͼ ��־
	bool CirclePlay;//ѭ�����ű������� ��־
	bool StopPlay;//ֹͣ���ű������� ��־
	bool swEngine;//��ʾ������Ϣ ��־
	int indexBdPic;//���̲���
	int indexBkPic;//����ͼƬ
	int indexBlcPer;//����ͼƬ
	int indexWhtPer;
	int BasePt;//���̵�ԭ��
}GameSet;

VOID SetText(HWND hWnd,LPCTSTR str);
VOID GetText(HWND hWnd,LPCTSTR str,int size);

class CChess
{
public:	
	HWND hWnd;//���ھ��

	GameSet *gameset;

	COLORREF BkColor;
	COLORREF BoardColor;
	RECT rtBoard;//����λ��
	int side;//���̿��
	int StepNum[2];//�ڰ�˫���Ĳ���
	int BasePt;//����ԭ���־
	char wMMsg[256];//��д�뱾��������Ϣ
	char wDMsg[256];//��д��Է�������Ϣ

	BYTE player;//���巽

	CChess();
	~CChess();
	//����������
	HFONT CreateSimpleFont(int width,int height);
	//���Ƶ�
	bool DrawPoint(HDC hDC,int r,COLORREF ptColor,COLORREF bkColor);	
	//���Ƹ���λͼ
	bool DrawAssist(HDC hAssistDC,int d);
	//�жϵ��Ƿ���������
	bool InsideRect(const RECT* rt,const int &x,const int &y);
};

#endif
