/**
 * file: Control.h
 * date: 2014/9/13
 * version: 1.0
 * description: 
 */


#ifndef CONTROL_H_GAMEPLATFORM
#define CONTROL_H_GAMEPLATFORM

#include "Game Platform.h"
#include "CEngine.h"


DWORD WINAPI EngineRun(LPVOID lpParam);//���������̺߳���

class Game
{
public:
	int GameMode;	//����ģʽ  -1:δ��ʼ 0����ִ�ڣ�����\����ִ�� 1����ִ�ף�����\����ִ�� 2���������� 3���˶���
					//4������ִ�ڣ�����ִ�� 5������ִ�ף�����ִ��

	Game();
	~Game();

	bool LoadEngine(int side);//�������棺ע��һ�����Ļ������
	bool UnloadEngine(int side);//ж������
	void StartGame();//���������߼�
	void StopGame();//��ֹ�����߼�
	bool MoveStep(int x,int y);//���壨�ṩ�û���
	bool MoveStep(char *step);//���壨�ṩ���棩
	void OkMove();
	void CancelMove();
	BOOL IsStop(){if(GameMode==-1)return TRUE;else return FALSE;}
	CEngine* GetBlackE(){return &BlackE;}//��ȡ�ڷ��������ָ��
	CEngine* GetWhiteE(){return &WhiteE;}//��ȡ�׷��������ָ��
private:
	CEngine BlackE,WhiteE;//�������
	CThread BlackT,WhiteT;//�̶߳���
};

extern Game game;


#endif