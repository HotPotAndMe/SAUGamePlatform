/**
 * file: Function.h
 * date: 2014/9/10
 * version: 1.0
 * description: 
 */


#ifndef MENU_H_GAMEPLATFORM
#define MMNU_H_GAMEPLATFORM


#include "Game Platform.h"


//ƽ̨�ṩ���ܽӿ�
class Fuction
{
public:
	static int Exit();//�˳�����
	static int LoadBlackEngine();//���غڷ�����
	static int UnloadBlackEngine();//ж�غڷ�����
	static int LoadWhiteEngine();//���ذ׷�����
	static int UnloadWhiteEngine();//ж�ذڷ�����
	static int GameStart();//��ʼ����
	static int GameStop();//��������
	static int MoveStep(int x, int y);//�û�����
	static int OkMove();
	static int CancelMove();
	static int CheckEngineLoad();
	static int ShotBoard();
	static int SaveChessManual(char *step);
};


#endif